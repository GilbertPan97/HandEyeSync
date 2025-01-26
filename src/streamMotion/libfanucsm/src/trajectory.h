//
// Created by sun on 2020/3/2.
//

#pragma once

#include <list>
#include <vector>
#include <Eigen/Core>

#include "raw_controller.h"


class Trajectory {
public:
    Trajectory()= default;
    virtual Eigen::VectorXd getPosition(double time) const = 0;
    virtual Eigen::VectorXd getVelocity(double time) const = 0;
    virtual double getDuration() const = 0;
    FANUC::RawController::RawPt operator()(double_t time);
};


class QuarticBlendJointTrajectory: public Trajectory{
public:
    QuarticBlendJointTrajectory(const std::list<Eigen::VectorXd> &path, const Eigen::VectorXd &maxVelocity, const Eigen::VectorXd &maxAcceleration, double minWayPointSeparation = 0.0);
    Eigen::VectorXd getPosition(double time) const override;
    Eigen::VectorXd getVelocity(double time) const override;
    double getDuration() const override;

private:
    std::vector<Eigen::VectorXd> path;
    std::vector<Eigen::VectorXd> velocities;
    std::vector<Eigen::VectorXd> accelerations;
    std::vector<double> durations;
    std::vector<double> blendDurations;
    double duration;
};

class QuarticBlendCartesianTrajectory: public Trajectory{
public:
    using Vector4d = Eigen::Matrix<double_t, 4, 1, Eigen::ColMajor>;
    QuarticBlendCartesianTrajectory(const std::list<Eigen::VectorXd> &path, const Vector4d &maxVelocity, const Vector4d &maxAcceleration, double minWayPointSeparation = 0.0);
    Eigen::VectorXd getPosition(double time) const override;
    Eigen::VectorXd getVelocity(double time) const override;
    double getDuration() const override;
private:
    std::vector<Eigen::VectorXd> path_;
    std::vector<Eigen::VectorXd> velocities;
    std::vector<Eigen::VectorXd> accelerations;
    std::vector<double> durations;
    std::vector<double> blendDurations;
    double duration;
};

enum InterpolationType{
    QBJOINT,
    QBCART
};

std::shared_ptr<Trajectory> trajectory_factory(const std::list<Eigen::VectorXd> &path, double_t factor, InterpolationType type);