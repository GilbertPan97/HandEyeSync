//
// Created by sun on 2020/2/27.
//

#pragma once

#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <thread>
#include <chrono>

#include <Eigen/Geometry>


#include "raw_controller.h"
#include "trajectory.h"


namespace FANUC{


    class StreamMotionController {
    public:
        const double_t RAD_TO_DEG = 180.0 / M_PI;
        const double_t DEG_TO_RAD = M_PI / 180.0;

        using IOType = RawController::IOType;
        StreamMotionController(const std::string& address,
                               const unsigned short int port = 60015):raw_controller_(address, port){

        };
        void start();
        void stop();
        
        Eigen::VectorXd getCurrentJointPosition();
        Eigen::Affine3d getCurrentCartesianPosition();
        Eigen::VectorXd getCurrentCartesianPositionXYZWPR();
        bool getIO(IOType type, uint16_t port_num);
        void setIO(IOType type, uint16_t port_num, bool value);
        bool isReady();
        bool isInMotion();
        
        void moveJoint(Eigen::VectorXd& joint_pos, double_t factor);
        void moveJoint(std::list<Eigen::VectorXd>& path, double factor);
        void moveCarteSian(Eigen::VectorXd& cartesian_pos, double_t factor);
        void moveCarteSian(std::list<Eigen::VectorXd>& path, double factor);
        void moveHome(double_t factor = 1.0);

        
        void loop();
        
    private:
        void getFSMState();
        void setFSMState();
        
        
    private:
        static void* run_thread(void* __this){
            auto* _this = (StreamMotionController* )__this;
            _this->raw_controller_.run();
            return nullptr;
        }



        RawController raw_controller_;
        pthread_t thread;
    };
}


