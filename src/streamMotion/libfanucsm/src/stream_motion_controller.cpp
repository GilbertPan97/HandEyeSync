//
// Created by sun on 2020/2/27.
//

#include "stream_motion_controller.h"

void FANUC::StreamMotionController::start() {
    /* we only need to start the raw controller's main loop*/
    /*TODO some real time stuff */
    int ret = pthread_create(&thread, nullptr, run_thread, (void*)this);
    if(ret){
        spdlog::info("[StreamMotionController] can not launch the controller thread");
    }
    /* wait util the raw controller is ready*/
    while(not raw_controller_.job_finished){

    }
    raw_controller_.job_finished =false;
}

void FANUC::StreamMotionController::loop() {
    int ret = pthread_join(thread,nullptr);
    if(ret){
        spdlog::info("[StreamMotionController] can not join the controller thread");
    }
}

void FANUC::StreamMotionController::stop() {
    raw_controller_.stop_req();
    while(not raw_controller_.job_finished){

    }
    raw_controller_.job_finished =false;
}

Eigen::VectorXd FANUC::StreamMotionController::getCurrentJointPosition() {
    RawController::Joint joint = raw_controller_.cur_joint();
    return (Eigen::VectorXd(6) << joint.j1 , joint.j2 , joint.j3 , joint.j4  , joint.j5, joint.j6).finished();
}

bool FANUC::StreamMotionController::getIO(FANUC::StreamMotionController::IOType type, uint16_t port_num) {
    return raw_controller_.read_io_req(type,port_num);
}

void FANUC::StreamMotionController::setIO(FANUC::StreamMotionController::IOType type, uint16_t port_num, bool value) {
    raw_controller_.write_io_req(type, port_num, value);
}

bool FANUC::StreamMotionController::isReady() {
    return raw_controller_.is_ready();
}

bool FANUC::StreamMotionController::isInMotion() {
    return raw_controller_.is_motion();
}

Eigen::Affine3d FANUC::StreamMotionController::getCurrentCartesianPosition() {
    auto world = raw_controller_.cur_world();
    Eigen::Matrix3d R;
    R = Eigen::AngleAxisd(world.r*DEG_TO_RAD, Eigen::Vector3d::UnitZ()) // static xyz
        * Eigen::AngleAxisd(world.p*DEG_TO_RAD, Eigen::Vector3d::UnitY())
        * Eigen::AngleAxisd(world.w*DEG_TO_RAD, Eigen::Vector3d::UnitX());
    Eigen::Affine3d ret = Eigen::Affine3d::Identity();
    ret.prerotate(R);
    (ret.translation() << world.x, world.y, world.z ).finished();
    return ret;
}

Eigen::VectorXd FANUC::StreamMotionController::getCurrentCartesianPositionXYZWPR() {
    auto world = raw_controller_.cur_world();
    return (Eigen::VectorXd(6) << world.x, world.y, world.z, world.w, world.p, world.r).finished();
}

void FANUC::StreamMotionController::moveJoint(std::list<Eigen::VectorXd> &path, double_t factor) {
    Eigen::VectorXd cur_joint = getCurrentJointPosition();
    path.push_front(cur_joint);
    auto trajectory  = trajectory_factory(path,factor,InterpolationType::QBJOINT);
    auto callback = std::bind(&Trajectory::operator(), trajectory, std::placeholders::_1);
    raw_controller_.joint_req(callback);
    while(not raw_controller_.job_finished){

    }
    raw_controller_.job_finished =false;
}

void FANUC::StreamMotionController::moveJoint(Eigen::VectorXd& joint_pos, double_t factor) {
    std::list<Eigen::VectorXd> path;
    Eigen::VectorXd cur_joint = getCurrentJointPosition();
    path.push_back(cur_joint);
    path.push_back(joint_pos);
    auto trajectory  = trajectory_factory(path,factor,InterpolationType::QBJOINT);
    auto callback = std::bind(&Trajectory::operator(), trajectory, std::placeholders::_1);
    raw_controller_.joint_req(callback);
    while(not raw_controller_.job_finished){

    }
    raw_controller_.job_finished =false;
}

void FANUC::StreamMotionController::moveCarteSian(Eigen::VectorXd &cartesian_pos, double_t factor) {
    std::list<Eigen::VectorXd> path;
    Eigen::VectorXd cur_joint = getCurrentCartesianPositionXYZWPR();
    path.push_back(cur_joint);
    path.push_back(cartesian_pos);
    auto trajectory  = trajectory_factory(path,factor,InterpolationType::QBCART);
    auto callback = std::bind(&Trajectory::operator(), trajectory, std::placeholders::_1);
    raw_controller_.cart_req(callback);
    while(not raw_controller_.job_finished){

    }
    raw_controller_.job_finished =false;
}

void FANUC::StreamMotionController::moveCarteSian(std::list<Eigen::VectorXd> &path, double factor) {
    Eigen::VectorXd cur_cart = getCurrentCartesianPositionXYZWPR();
    path.push_front(cur_cart);
    auto  trajectory = trajectory_factory(path,factor,InterpolationType::QBCART);
    auto callback = std::bind(&Trajectory::operator(), trajectory, std::placeholders::_1);
    raw_controller_.cart_req(callback);
    while(not raw_controller_.job_finished){

    }
    raw_controller_.job_finished =false;
}

void FANUC::StreamMotionController::moveHome(double_t factor) {
    Eigen::VectorXd goal = (Eigen::VectorXd(6) << 0,0,0,0,-90,0).finished();
    moveJoint(goal, factor);
}





