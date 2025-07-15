//
// Created by sun on 2020/3/2.
//
#include <iterator>
#include "trajectory.h"

using namespace std;
using namespace Eigen;

QuarticBlendJointTrajectory::QuarticBlendJointTrajectory(const std::list<Eigen::VectorXd> &path_,
                                                         const Eigen::VectorXd &maxVelocity,
                                                         const Eigen::VectorXd &maxAcceleration,
                                                         double minWayPointSeparation) :
        path(path_.begin(), path_.end()),
        velocities(path.size() - 1),
        accelerations(path.size()),
        durations(path.size() - 1),
        blendDurations(path.size()),
        duration(0.0) {
    bool removeWayPoints = false;
    if (minWayPointSeparation > 0.0) {
        removeWayPoints = true;
    }

    // remove waypoints that are too close together
    while (removeWayPoints) {
        removeWayPoints = false;

        vector<VectorXd>::iterator it = path.begin();
        vector<VectorXd>::iterator next = it;
        if (next != path.end())
            next++;
        while (next != path.end()) {
            if ((*it - *next).norm() < minWayPointSeparation) {
                removeWayPoints = true;
                vector<VectorXd>::iterator nextNext = next;
                nextNext++;
                if (it == path.begin()) {
                    it = path.erase(next);
                } else if (nextNext == path.end()) {
                    it = path.erase(it);
                } else {
                    VectorXd newViaPoint = 0.5 * (*it + *next);
                    it = path.erase(it);
                    it = path.insert(it, newViaPoint);
                    it++;
                    it = path.erase(it);
                }
                next = it;
                if (next != path.end())
                    next++;
            } else {
                it = next;
                next++;
            }
        }

        velocities.resize(path.size() - 1);
        accelerations.resize(path.size());
        durations.resize(path.size() - 1);
        blendDurations.resize(path.size());
    }

    // calculate time between waypoints and initial velocities of linear segments
    for (unsigned int i = 0; i < path.size() - 1; i++) {
        durations[i] = 0.0;
        for (int j = 0; j < path[i].size(); j++) {
            durations[i] = max(durations[i], abs(path[i + 1][j] - path[i][j]) / maxVelocity[j]);
        }
        velocities[i] = (path[i + 1] - path[i]) / durations[i];
    }

    int numBlendsSlowedDown = numeric_limits<int>::max();
    while (numBlendsSlowedDown > 1) {
        numBlendsSlowedDown = 0;
        vector<double> slowDownFactors(path.size(), 1.0);

        for (unsigned int i = 0; i < path.size(); i++) {
            // calculate blend duration and acceleration
            VectorXd previousVelocity = (i == 0) ? VectorXd::Zero(path[i].size()) : velocities[i - 1];
            VectorXd nextVelocity = (i == path.size() - 1) ? VectorXd::Zero(path[i].size()) : velocities[i];
            blendDurations[i] = 0.0;
            for (int j = 0; j < path[i].size(); j++) {
                blendDurations[i] = max(blendDurations[i],
                                        3.0 / 2.0 * abs(nextVelocity[j] - previousVelocity[j]) / maxAcceleration[j]);
                accelerations[i] = 3.0 / 2.0 * (nextVelocity - previousVelocity) / blendDurations[i];
            }

            // calculate slow down factor such that the blend phase replaces at most half of the neighboring linear segments
            const double eps = 0.000001;
            if ((i > 0 && blendDurations[i] > durations[i - 1] + eps &&
                 blendDurations[i - 1] + blendDurations[i] > 2.0 * durations[i - 1] + eps)
                || i < path.size() - 1 && blendDurations[i] > durations[i] + eps &&
                   blendDurations[i] + blendDurations[i + 1] > 2.0 * durations[i] + eps) {
                numBlendsSlowedDown++;
                const double maxDuration = min(i == 0 ? numeric_limits<double>::max() : durations[i - 1],
                                               i == path.size() - 1 ? numeric_limits<double>::max() : durations[i]);
                slowDownFactors[i] = sqrt(maxDuration / blendDurations[i]);
            }
        }

        // apply slow down factors to linear segments
        for (unsigned int i = 0; i < path.size() - 1; i++) {
            velocities[i] *= min(slowDownFactors[i], slowDownFactors[i + 1]);
            durations[i] /= min(slowDownFactors[i], slowDownFactors[i + 1]);
        }
    }

    // calculate total time of trajectory
    for (unsigned int i = 0; i < path.size() - 1; i++) {
        duration += durations[i];
    }
    duration += 0.5 * blendDurations.front() + 0.5 * blendDurations.back();

}

Eigen::VectorXd QuarticBlendJointTrajectory::getPosition(double time) const {
    if (time > duration) {
        return path.back();
    }
    double t = time;
    if (t <= 0.5 * blendDurations[0]) {
        return path[0] -
               1.0 / (2.0 * std::pow(blendDurations[0], 3)) * t * t * t * (t - 2 * blendDurations[0]) * velocities[0];
    } else {
        t -= 0.5 * blendDurations[0];
    }
    unsigned int i = 0;
    while (i < path.size() - 1 && t > durations[i]) {
        t -= durations[i];
        i++;
    }
    if (i == path.size() - 1) {
        t = 0.5 * blendDurations.back() - t;
        return path.back() +
               1.0 / (2.0 * std::pow(blendDurations.back(), 3)) * t * t * t * (t - 2 * blendDurations.back()) *
               velocities.back();
    }

    double switchingTime1 = 0.5 * blendDurations[i];
    double switchingTime2 = durations[i] - 0.5 * blendDurations[i + 1];

    if (t < switchingTime1) {
        t = switchingTime1 - t;
        return path[i] + switchingTime1 * velocities[i] - t * velocities[i] -
               1.0 / (2.0 * std::pow(blendDurations[i], 3)) * t * t * t * (t - 2 * blendDurations[i]) *
               (velocities[i] - ((i == 0) ? VectorXd::Zero(path[i].size()) : velocities[i - 1]));
    } else if (t > switchingTime2) {
        t -= switchingTime2;
        return path[i] + switchingTime2 * velocities[i] + t * velocities[i] -
               1.0 / (2.0 * std::pow(blendDurations[i + 1], 3)) * t * t * t * (t - 2 * blendDurations[i + 1]) *
               (((i == path.size() - 2) ? VectorXd::Zero(path[i].size()) : velocities[i + 1]) - velocities[i]);
    } else {
        return path[i] + t * velocities[i];
    }
}

Eigen::VectorXd QuarticBlendJointTrajectory::getVelocity(double time) const {
    if (time > duration) {
        return VectorXd::Zero(path.back().size());
    }
    double t = time;
    if (t <= 0.5 * blendDurations[0]) {
        return -2.0 / std::pow(blendDurations[0], 3) * t * t * (t - 1.5 * blendDurations[0]) * velocities[0];
    } else {
        t -= 0.5 * blendDurations[0];
    }
    unsigned int i = 0;
    while (i < path.size() - 1 && t > durations[i]) {
        t -= durations[i];
        i++;
    }
    if (i == path.size() - 1) {
        t = 0.5 * blendDurations.back() - t;
        return -2.0 / std::pow(blendDurations.back(), 3) * t * t * (t - 1.5 * blendDurations.back()) *
               velocities.back();
    }

    double switchingTime1 = 0.5 * blendDurations[i];
    double switchingTime2 = durations[i] - 0.5 * blendDurations[i + 1];

    if (t < switchingTime1) {
        t = switchingTime1 - t;
        return velocities[i] + 2.0 / std::pow(blendDurations[i], 3) * t * t * (t - 1.5 * blendDurations[i]) *
                               (velocities[i] - ((i == 0) ? VectorXd::Zero(path[i].size()) : velocities[i - 1]));
    } else if (t > switchingTime2) {
        t -= switchingTime2;
        return velocities[i] - 2.0 / std::pow(blendDurations[i + 1], 3) * t * t * (t - 1.5 * blendDurations[i + 1]) *
                               (((i == path.size() - 2) ? VectorXd::Zero(path[i].size()) : velocities[i + 1]) -
                                velocities[i]);
    } else {
        return velocities[i];
    }
}

double QuarticBlendJointTrajectory::getDuration() const {
    return duration;
}


QuarticBlendCartesianTrajectory::QuarticBlendCartesianTrajectory(const list<Eigen::VectorXd> &path,
                                                                 const Vector4d &maxVelocity,
                                                                 const Vector4d &maxAcceleration,
                                                                 double minWayPointSeparation) :
        path_(path.begin(), path.end()),
        velocities(path_.size() - 1),
        accelerations(path_.size()),
        durations(path_.size() - 1),
        blendDurations(path_.size()),
        duration(0.0) {

    bool removeWayPoints = false;
    if (minWayPointSeparation > 0.0) {
        removeWayPoints = true;
    }

    // remove waypoints that are too close together
    while (removeWayPoints) {
        removeWayPoints = false;

        vector<Eigen::VectorXd>::iterator it = path_.begin();
        vector<Eigen::VectorXd>::iterator next = it;
        if (next != path_.end())
            next++;
        while (next != path_.end()) {
            if (((*it).head(3) - (*next).head(3)).norm() < minWayPointSeparation) {
                removeWayPoints = true;
                vector<Eigen::VectorXd>::iterator nextNext = next;
                nextNext++;
                if (it == path_.begin()) {
                    it = path_.erase(next);
                } else if (nextNext == path_.end()) {
                    it = path_.erase(it);
                } else {
                    VectorXd newViaPoint = 0.5 * (*it + *next);
                    it = path_.erase(it);
                    it = path_.insert(it, newViaPoint);
                    it++;
                    it = path_.erase(it);
                }
                next = it;
                if (next != path_.end())
                    next++;
            } else {
                it = next;
                next++;
            }
        }

        velocities.resize(path_.size() - 1);
        accelerations.resize(path_.size());
        durations.resize(path_.size() - 1);
        blendDurations.resize(path_.size());
    }

    // calculate time between waypoints and initial velocities of linear segments
    for (unsigned int i = 0; i < path_.size() - 1; i++) {
        durations[i] = 0.0;
        // translation
        durations[i] = max(durations[i], (path_[i + 1].head(3) - path_[i].head(3)).norm() / maxVelocity[0]);
        // rotation euler angles
        for (int j = 3; j < path_[i].size(); j++) {
            durations[i] = max(durations[i], abs(path_[i + 1][j] - path_[i][j]) / maxVelocity[j - 3]);
        }
        velocities[i] = (path_[i + 1] - path_[i]) / durations[i];
    }

    int numBlendsSlowedDown = numeric_limits<int>::max();
    while (numBlendsSlowedDown >= 1) { // here we make it check if the blend have changed
        numBlendsSlowedDown = 0;
        vector<double> slowDownFactors(path_.size(), 1.0);

        for (unsigned int i = 0; i < path_.size(); i++) {
            // calculate blend duration and acceleration
            Eigen::VectorXd previousVelocity = (i == 0) ? VectorXd::Zero(path_[i].size()) : velocities[i - 1];
            Eigen::VectorXd nextVelocity = (i == path_.size() - 1) ? VectorXd::Zero(path_[i].size()) : velocities[i];
            blendDurations[i] = 0.0;
            // translation
            blendDurations[i] = max(blendDurations[i],
                                    3.0 / 2.0 * (nextVelocity.head(3) - previousVelocity.head(3)).norm() /
                                    maxAcceleration[0]);
            accelerations[i] = 3.0 / 2.0 * (nextVelocity - previousVelocity) / blendDurations[i];
            // rotation
            for (int j = 3; j < path_[i].size(); j++) {
                blendDurations[i] = max(blendDurations[i], 3.0 / 2.0 * abs(nextVelocity[j] - previousVelocity[j]) /
                                                           maxAcceleration[j - 3]);
                accelerations[i] = 3.0 / 2.0 * (nextVelocity - previousVelocity) / blendDurations[i];
            }

            // calculate slow down factor such that the blend phase replaces at most half of the neighboring linear segments
            const double eps = 0.000001;
            if ((i > 0 && blendDurations[i] > durations[i - 1] + eps &&
                 blendDurations[i - 1] + blendDurations[i] > 2.0 * durations[i - 1] + eps)
                || i < path_.size() - 1 && blendDurations[i] > durations[i] + eps &&
                   blendDurations[i] + blendDurations[i + 1] > 2.0 * durations[i] + eps) {
                numBlendsSlowedDown++;
                const double maxDuration = min(i == 0 ? numeric_limits<double>::max() : durations[i - 1],
                                               i == path_.size() - 1 ? numeric_limits<double>::max() : durations[i]);
                slowDownFactors[i] = sqrt(maxDuration / blendDurations[i]);
            }
        }

        // apply slow down factors to linear segments
        for (unsigned int i = 0; i < path_.size() - 1; i++) {
            velocities[i] *= min(slowDownFactors[i], slowDownFactors[i + 1]);
            durations[i] /= min(slowDownFactors[i], slowDownFactors[i + 1]);
        }
    }

    // calculate total time of trajectory
    for (unsigned int i = 0; i < path_.size() - 1; i++) {
        duration += durations[i];
    }
    duration += 0.5 * blendDurations.front() + 0.5 * blendDurations.back();

}

Eigen::VectorXd QuarticBlendCartesianTrajectory::getPosition(double time) const {
    if (time > duration) {
        return path_.back();
    }
    double t = time;
    if (t <= 0.5 * blendDurations[0]) {
        return path_[0] -
               1.0 / (2.0 * std::pow(blendDurations[0], 3)) * t * t * t * (t - 2 * blendDurations[0]) * velocities[0];
    } else {
        t -= 0.5 * blendDurations[0];
    }
    unsigned int i = 0;
    while (i < path_.size() - 1 && t > durations[i]) {
        t -= durations[i];
        i++;
    }
    if (i == path_.size() - 1) {
        t = 0.5 * blendDurations.back() - t;
        return path_.back() +
               1.0 / (2.0 * std::pow(blendDurations.back(), 3)) * t * t * t * (t - 2 * blendDurations.back()) *
               velocities.back();
    }

    double switchingTime1 = 0.5 * blendDurations[i];
    double switchingTime2 = durations[i] - 0.5 * blendDurations[i + 1];

    if (t < switchingTime1) {
        t = switchingTime1 - t;
        return path_[i] + switchingTime1 * velocities[i] - t * velocities[i] -
               1.0 / (2.0 * std::pow(blendDurations[i], 3)) * t * t * t * (t - 2 * blendDurations[i]) *
               (velocities[i] - ((i == 0) ? VectorXd::Zero(path_[i].size()) : velocities[i - 1]));
    } else if (t > switchingTime2) {
        t -= switchingTime2;
        return path_[i] + switchingTime2 * velocities[i] + t * velocities[i] -
               1.0 / (2.0 * std::pow(blendDurations[i + 1], 3)) * t * t * t * (t - 2 * blendDurations[i + 1]) *
               (((i == path_.size() - 2) ? VectorXd::Zero(path_[i].size()) : velocities[i + 1]) - velocities[i]);
    } else {
        return path_[i] + t * velocities[i];
    }
}

Eigen::VectorXd QuarticBlendCartesianTrajectory::getVelocity(double time) const {
    if (time > duration) {
        return VectorXd::Zero(path_.back().size());
    }
    double t = time;
    if (t <= 0.5 * blendDurations[0]) {
        return -2.0 / std::pow(blendDurations[0], 3) * t * t * (t - 1.5 * blendDurations[0]) * velocities[0];
    } else {
        t -= 0.5 * blendDurations[0];
    }
    unsigned int i = 0;
    while (i < path_.size() - 1 && t > durations[i]) {
        t -= durations[i];
        i++;
    }
    if (i == path_.size() - 1) {
        t = 0.5 * blendDurations.back() - t;
        return -2.0 / std::pow(blendDurations.back(), 3) * t * t * (t - 1.5 * blendDurations.back()) *
               velocities.back();
    }

    double switchingTime1 = 0.5 * blendDurations[i];
    double switchingTime2 = durations[i] - 0.5 * blendDurations[i + 1];

    if (t < switchingTime1) {
        t = switchingTime1 - t;
        return velocities[i] + 2.0 / std::pow(blendDurations[i], 3) * t * t * (t - 1.5 * blendDurations[i]) *
                               (velocities[i] - ((i == 0) ? VectorXd::Zero(path_[i].size()) : velocities[i - 1]));
    } else if (t > switchingTime2) {
        t -= switchingTime2;
        return velocities[i] - 2.0 / std::pow(blendDurations[i + 1], 3) * t * t * (t - 1.5 * blendDurations[i + 1]) *
                               (((i == path_.size() - 2) ? VectorXd::Zero(path_[i].size()) : velocities[i + 1]) -
                                velocities[i]);
    } else {
        return velocities[i];
    }
}

double QuarticBlendCartesianTrajectory::getDuration() const {
    return duration;
}


FANUC::RawController::RawPt Trajectory::operator()(double_t time) {
    FANUC::RawController::RawPt ret{};
    auto joint = getPosition(time);
    ret.joint.j1 = joint[0];
    ret.joint.j2 = joint[1];
    ret.joint.j3 = joint[2];
    ret.joint.j4 = joint[3];
    ret.joint.j5 = joint[4];
    ret.joint.j6 = joint[5];
    if (time > getDuration()) {
        ret.finished = true;
    }
    return ret;
}

std::shared_ptr<Trajectory> trajectory_factory(const std::list<Eigen::VectorXd> &path, double_t factor, InterpolationType type) {

    std::shared_ptr<Trajectory> trajectory;
    switch (type) {
        case InterpolationType::QBJOINT:
            trajectory = std::make_shared<QuarticBlendJointTrajectory>(path, factor * (Eigen::VectorXd(6)
                                                                 << 370.0, 310.0, 410.0, 550.0, 545.0, 1000.0).finished(),
                                                         factor * (Eigen::VectorXd(6)
                                                                 << 770, 640, 1000, 2000, 2100, 1770).finished());
            break;
        case InterpolationType::QBCART:
            trajectory = std::make_shared<QuarticBlendCartesianTrajectory>(path, factor * (QuarticBlendCartesianTrajectory::Vector4d()
                                                                     << 300, 300, 300, 300).finished(),
                                                             factor * (QuarticBlendCartesianTrajectory::Vector4d()
                                                                     << 600, 600, 600, 600).finished());
    }
    return trajectory;
}

