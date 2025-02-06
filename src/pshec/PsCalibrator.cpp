#include "PsCalibrator.h"
#include "common.h"
#include "utils.h"
#include "PsAlgorithm.h"

#include <iostream>
#include <cmath>
#include <numeric>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <unsupported/Eigen/KroneckerProduct>

// Utils is visible for all Calibrator space
using namespace CalibUtils; 

namespace ProfileScanner
{
    HandEyeCalib::HandEyeCalib(){

    }

    HandEyeCalib::HandEyeCalib(CalibType type){
        type_ = type;
    }

    HandEyeCalib::~HandEyeCalib(){

    }

    void HandEyeCalib::SetCalibType(CalibType type) {
        type_ = type;
    }

    bool HandEyeCalib::SetRobPose(const std::vector<Eigen::Vector<float, 6>> rob_pose){
        if(rob_pose.size()==0){
            std::cout << "ERROR: robot pose data is empty. \n";
            return false;
        }
        nbr_data_ = rob_pose.size();
        vec_end2base_ = rob_pose;

        // generate mtr_end2base_
        Eigen::Matrix4f tmp_HTM;
        for (const Eigen::Vector<float, 6>& xyzwpr: vec_end2base_){
            tmp_HTM = CalibUtils::XYZWPRVecToHTM(xyzwpr);
            mtr_end2base_.push_back(tmp_HTM);
        }
        return true;
    }

    bool HandEyeCalib::SetRobPose(const std::vector<Eigen::Vector<float, 6>> rob_pose,
                                    Eigen::Vector<float, 6> ref_frame){
        if(rob_pose.size()==0){
            std::cout << "ERROR: robot pose data is empty. \n";
            return false;
        }
        nbr_data_ = rob_pose.size();
        vec_end2base_ = rob_pose;

        // generate mtr_end2base_
        Eigen::Matrix4f tmp_HTM;
        for (const Eigen::Vector<float, 6>& xyzwpr: vec_end2base_){
            tmp_HTM = CalibUtils::XYZWPRVecToHTM(ref_frame) * CalibUtils::XYZWPRVecToHTM(xyzwpr);
            mtr_end2base_.push_back(tmp_HTM);
        }
        return true;
    }

    bool HandEyeCalib::SetProfileData(std::vector<cv::Point3f> pnts_data, CalibObj obj){
        // check the volume of the dataset
        if (nbr_data_ != pnts_data.size()){
            std::cout << "WARNING: Number of lines not match nbr_data_. \n";
            nbr_data_ = pnts_data.size();
        }

        calib_obj_ = obj;

        if (obj == CalibObj::SPHERE)
            ctr_pnts_ = pnts_data;
        else if(obj == CalibObj::BLOCK)
            edge_pnts_ = pnts_data;
        else {
            std::cout << "ERROR: calibration objection is false.\n";
            return false;
        }
        
        return true;
    }

    bool HandEyeCalib::run(SolveMethod method){
        // check calibration dataset
        if(nbr_data_ != mtr_end2base_.size()){
            std::cout << "ERROR: Error in data number";
            return false;
        }

        // construct algorithm parser and calculate result
        Eigen::Matrix3f Rx;
        Eigen::Vector3f tx;
        if (calib_obj_ == CalibObj::SPHERE) {
            algorithm algor(Constructor::PNT_CONSTANT, method);
            std::vector<Eigen::Matrix4f> mtr_rob;
            if (type_ == CalibType::EYE_IN_HAND)
                mtr_rob = mtr_end2base_;
            else if (type_ == CalibType::EYE_TO_HAND){
                for (const auto & htm: mtr_end2base_)
                    mtr_rob.push_back(htm.inverse());
            }
                
            algor.Simultaneous_Calib(mtr_rob, toEigenPoints(ctr_pnts_), Rx, tx);
        } 
        else if(calib_obj_ == CalibObj::BLOCK) {
            // BUG: temp half dataset used for calibrate rotation component
            std::vector<Eigen::Matrix4f> htm_end2base_r;
            std::vector<Eigen::Vector3f> p_cam_r;
            std::vector<Eigen::Matrix4f> htm_end2base_t;
            std::vector<Eigen::Vector3f> p_cam_t;
            for (size_t i = 0; i < mtr_end2base_.size(); i++)
            {
                Eigen::Matrix4f mtr_end2base_i = mtr_end2base_[i];
                Eigen::Vector3f p_cam_i = {edge_pnts_[i].x, 
                                           edge_pnts_[i].y, 
                                           edge_pnts_[i].z};
                if (i < mtr_end2base_.size() / 2) {
                    htm_end2base_r.emplace_back(mtr_end2base_i);
                    p_cam_r.emplace_back(p_cam_i);
                }
                else {
                    htm_end2base_t.emplace_back(mtr_end2base_i);
                    p_cam_t.emplace_back(p_cam_i);
                }
            }
            algorithm algor(Constructor::LINE_PARALLEL, method);
            algor.Separate_Calib(htm_end2base_r, p_cam_r, htm_end2base_t, p_cam_t, Rx, tx);
        }

        // save calib result to mtr_cam2rob_ and vec_cam2rob_
        mtr_cam2rob_ = CalibUtils::RT2HomogeneousMatrix(Rx, tx);
        vec_cam2rob_ = CalibUtils::HTMToXYZWPRVec(mtr_cam2rob_);

        std::cout << "INFO: Calibration result in matrix format: \n"
                    << mtr_cam2rob_ << std::endl;
        std::cout << "INFO: Calibration result in XYZWPR format: \n"
                    << vec_cam2rob_.transpose() << std::endl;

        return true;
    }

    Eigen::Matrix4f HandEyeCalib::GetCalcResult(){
        return mtr_cam2rob_;
    }

    float HandEyeCalib::CalcCalibError(){
        // check calibration restlt
        if (mtr_cam2rob_.hasNaN()){
            std::cout << "ERROR: Calib result is empty." << std::endl;
            exit(-3);
        }

        // calculate sphere center points (in rob base frame)
        Eigen::Vector3f ctrPnt_cam;
        Eigen::Vector3f aveCtrPntBase = {0.0f, 0.0f, 0.0f};
        std::vector<Eigen::Vector3f> ctrPnts_base;
        std::vector<Eigen::Matrix4f> mtr_rob;
        if (type_ == CalibType::EYE_IN_HAND){
            mtr_rob = mtr_end2base_;
            std::cout << "INFO: Sphere center coordination in robot base frame is: \n";
        }
        else if (type_ == CalibType::EYE_TO_HAND) {
            for (const auto & mtr: mtr_end2base_)
                mtr_rob.push_back(mtr.inverse());
            std::cout << "INFO: Sphere center coordination in robot flange frame is: \n";
        }
        else{
            std::cerr << "ERROR: Calibration type not defined.\n";
        }

        for (size_t i = 0; i < mtr_rob.size(); i++)
        {
            ctrPnt_cam << ctr_pnts_[i].x, ctr_pnts_[i].y, ctr_pnts_[i].z;

            auto ctrPnt_base = (mtr_rob[i] * mtr_cam2rob_ * 
                (Eigen::Vector4f() << ctrPnt_cam, 1.0f).finished()).segment<3>(0);

            aveCtrPntBase += ctrPnt_base;
            std::cout << "  " << ctrPnt_base.transpose() << std::endl;
            ctrPnts_base.push_back(ctrPnt_base);
        }
        aveCtrPntBase /= mtr_rob.size();
        std::cout << "INFO: The sphere center average coordination is:"
                    << aveCtrPntBase.transpose() << std::endl;

        // calculate the standard deviation of the distance from robot base to sphere center
        float calib_error_ = CalcPntStandDeviation(ctrPnts_base);

        return calib_error_;
    }

    float HandEyeCalib::CalcCalibError(std::vector<cv::Point3f> feaPnts_camera,
                                        std::vector<Eigen::Matrix4f> robPoses){
        // calculate feature points in robot base 
        std::cout << "INFO: Feature point coordination in robot base frame is: \n";
        std::vector<Eigen::Vector3f> feaPnts_base;
        for (size_t i = 0; i < feaPnts_camera.size(); i++){
            Eigen::Vector3f feaPnt(feaPnts_camera[i].x, 
                                   feaPnts_camera[i].y, 
                                   feaPnts_camera[i].z);
            auto feaPnt_base = (robPoses[i] * mtr_cam2rob_ *
                (Eigen::Vector4f() << feaPnt, 1.0f).finished()).segment<3>(0);

            std::cout << "  " << feaPnt_base.transpose() << std::endl;
            feaPnts_base.push_back(feaPnt_base);
        }

        // calculate the standard deviation of the distance from robot base to block feature point
        float calib_error_ = CalcPntStandDeviation(feaPnts_base);

        return calib_error_;
    }

    void HandEyeCalib::drawClusters(cv::Mat& img, std::vector<cv::Point2f>& centers, 
                                    std::vector<std::vector<cv::Point2f>>& points) {
        // draw cluster center
        for (int i = 0; i < centers.size(); i++) {
            circle(img, centers[i], 5, cv::Scalar(0, 0, 255), -1);
        }

        // draw point cloud of circle
        for (int i = 0; i < points.size(); i++) {
            cv::Scalar color(rand() % 256, rand() % 256, rand() % 256);
            for (int j = 0; j < points[i].size(); j++)
                circle(img, points[i][j], 2, color, -1);
        }
    }

    float HandEyeCalib::CalcPntStandDeviation(std::vector<Eigen::Vector3f> pntSet3d){
        // calculate the standard deviation of the distance from robot base to spherecenter point
        std::cout << "INFO: The distance of frature point in robot base frame is: \n";
        std::vector<float> dist_ctr_pnts;
        for (const auto& point : pntSet3d) {
            float dist = std::sqrt(point.dot(point));
            dist_ctr_pnts.push_back(dist);
            std::cout << "  " << dist;
        }
        std::cout << std::endl;

        float mean = std::accumulate(dist_ctr_pnts.begin(), dist_ctr_pnts.end(), 0.0) / dist_ctr_pnts.size();
        float variance = 0.0;
        for (auto x : dist_ctr_pnts)
            variance += std::pow(x - mean, 2);
        float error = std::sqrt(variance / (dist_ctr_pnts.size() - 1));
        std::cout << "INFO: Standard deviation is: " << error << std::endl;

        return error;
    }
} // namespace ProfileScanner
