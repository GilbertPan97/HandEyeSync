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

    bool HandEyeCalib::SetProfileData(std::vector<cv::Point3f> pnts_data, CalibObj obj, std::vector<int> labels) {
        // Check if the input dataset size matches the expected number of data points
        if (nbr_data_ != pnts_data.size()) {
            std::cout << "WARNING: Number of points does not match nbr_data_. Updating nbr_data_.\n";
            nbr_data_ = pnts_data.size();
        }
    
        // Set calibration object type
        calib_obj_ = obj;
    
        if (obj == CalibObj::SPHERE) {
            // If the calibration object is a sphere, store the center points
            ctr_pnts_ = std::move(pnts_data);
        } 
        else if (obj == CalibObj::EDGE) {
            // If the calibration object is an edge, store the edge points
            edge_pnts_ = std::move(pnts_data);
            
            // Ensure that valid labels are provided for edge-based calibration
            if (!labels.empty()) {
                data_labels_ = std::move(labels);
            } else {
                throw std::runtime_error("ERROR: Missing dataset labels for edge calibration algorithm.");
            }
        } 
        else {
            // Invalid calibration object type
            std::cerr << "ERROR: Invalid calibration object type.\n";
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

        // Reconstruct calibration data for EYE_IN_HAND and EYE_TO_HAND
        std::vector<Eigen::Matrix4f> mtr_rob;
        if (type_ == CalibType::EYE_IN_HAND)
            mtr_rob = mtr_end2base_;
        else if (type_ == CalibType::EYE_TO_HAND){
            for (const auto & htm: mtr_end2base_)
                mtr_rob.push_back(htm.inverse());
        }

        // construct algorithm parser and calculate result
        Eigen::Matrix3f Rx;
        Eigen::Vector3f tx;
        if (calib_obj_ == CalibObj::SPHERE) {
            algorithm algor(Constructor::PNT_CONSTANT, method);    
            algor.Simultaneous_Calib(mtr_rob, toEigenPoints(ctr_pnts_), Rx, tx);
        } 
        else if(calib_obj_ == CalibObj::EDGE) {
            // Allocate calibration dataset for rotational and translational components
            std::vector<Eigen::Matrix4f> htm_end2base_r;
            std::vector<Eigen::Vector3f> p_cam_r;
            std::vector<Eigen::Matrix4f> htm_end2base_t;
            std::vector<Eigen::Vector3f> p_cam_t;
            for (size_t i = 0; i < mtr_end2base_.size(); i++)
            {
                Eigen::Matrix4f mtr_end2base_i = mtr_end2base_[i];
                Eigen::Vector3f p_cam_i = {edge_pnts_[i].x, edge_pnts_[i].y, edge_pnts_[i].z};
                if (data_labels_[i] == 0) {
                    htm_end2base_r.emplace_back(mtr_end2base_i);
                    p_cam_r.emplace_back(p_cam_i);
                }
                else if (data_labels_[i] == 1) {
                    htm_end2base_t.emplace_back(mtr_end2base_i);
                    p_cam_t.emplace_back(p_cam_i);
                }
                else {
                    throw std::runtime_error("Unknow data label for index:" + std::to_string(i));
                }
            }
            algorithm algor(Constructor::LINE_PARALLEL, method);
            algor.Separate_Calib(htm_end2base_r, p_cam_r, htm_end2base_t, p_cam_t, Rx, tx);
        }

        // save calib result to mtr_cam2rob_ and vec_cam2rob_
        mtr_cam2rob_ = CalibUtils::RT2HomogeneousMatrix(Rx, tx);
        vec_cam2rob_ = CalibUtils::HTMToXYZWPRVec(mtr_cam2rob_);

        return true;
    }
    Eigen::Matrix4f HandEyeCalib::GetCalcResult() const {
        if (mtr_cam2rob_.isZero()) {
            // Return identity matrix if uninitialized
            return Eigen::Matrix4f::Identity();
        }
        return mtr_cam2rob_;
    }
    
    Eigen::Vector<float, 6> HandEyeCalib::GetCalcResultVec() const {
        if (vec_cam2rob_.isZero()) {
            // Return zero vector if uninitialized
            return Eigen::Vector<float, 6>::Zero();
        }
        return vec_cam2rob_;
    }

    float HandEyeCalib::CalcCalibError(std::string type){
        // check calibration restlt
        if (mtr_cam2rob_.hasNaN()){
            std::cout << "ERROR: Calib result is empty." << std::endl;
            throw std::runtime_error("Calibration result is empty");
        }

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

        if (type == "Sphere") {
            // calculate sphere center points (in rob base frame)
            Eigen::Vector3f ctrPnt_cam;
            Eigen::Vector3f aveCtrPntBase = {0.0f, 0.0f, 0.0f};
            std::vector<Eigen::Vector3f> ctrPnts_base;

            for (size_t i = 0; i < mtr_rob.size(); i++){
                ctrPnt_cam << ctr_pnts_[i].x, ctr_pnts_[i].y, ctr_pnts_[i].z;

                auto ctrPnt_base = (mtr_rob[i] * mtr_cam2rob_ * 
                    (Eigen::Vector4f() << ctrPnt_cam, 1.0f).finished()).segment<3>(0);

                aveCtrPntBase += ctrPnt_base;
                ctrPnts_base.push_back(ctrPnt_base);
            }
            ctr_pnts_base_ = convertToCvPoint3f(ctrPnts_base);

            aveCtrPntBase /= mtr_rob.size();
            std::cout << "INFO: The sphere center average coordination is:"
                        << aveCtrPntBase.transpose() << std::endl;

            // calculate the standard deviation of the distance from robot base to sphere center
            calib_error_ = CalcPntStandDeviation(ctrPnts_base);
        }
        else if (type == "Edge") {
            Eigen::Vector3f edgePnt_cam;
            std::vector<Eigen::Vector3f> edgePnts_base;

            for (size_t i = 0; i < mtr_rob.size(); i++) {
                edgePnt_cam << edge_pnts_[i].x, edge_pnts_[i].y, edge_pnts_[i].z;

                auto edgePnt_base = (mtr_rob[i] * mtr_cam2rob_ * 
                    (Eigen::Vector4f() << edgePnt_cam, 1.0f).finished()).segment<3>(0);

                edgePnts_base.push_back(edgePnt_base);
            }

            edge_pnts_base_ = convertToCvPoint3f(edgePnts_base);
            calib_error_ = calcCollinearityError(edgePnts_base);  
        }
        else {
            throw std::runtime_error("Unknown Calibration Model.");
        }

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

    float HandEyeCalib::calcCollinearityError(const std::vector<Eigen::Vector3f>& points) {
        if (points.size() < 2) {
            return 0.0f; // Less than two points, no error to compute
        }
    
        // Compute centroid of the points
        Eigen::Vector3f centroid(0, 0, 0);
        for (const auto& pt : points) {
            centroid += pt;
        }
        centroid /= static_cast<float>(points.size());
    
        // Assemble the covariance matrix
        Eigen::MatrixXf centered(points.size(), 3);
        for (size_t i = 0; i < points.size(); ++i) {
            centered.row(i) = points[i] - centroid;
        }
    
        // Perform SVD to find the direction vector of the best-fit line
        Eigen::JacobiSVD<Eigen::MatrixXf> svd(centered, Eigen::ComputeThinU | Eigen::ComputeThinV);
        Eigen::Vector3f direction = svd.matrixV().col(0).normalized();
    
        // Calculate the distance of each point to the fitted line
        float sumSquaredError = 0.0f;
        for (const auto& pt : points) {
            Eigen::Vector3f diff = pt - centroid;
            float distance = (diff - diff.dot(direction) * direction).norm();
            sumSquaredError += distance * distance;
        }
    
        // Compute RMSE
        return std::sqrt(sumSquaredError / static_cast<float>(points.size()));
    }

    std::vector<cv::Point3f> HandEyeCalib::convertToCvPoint3f(const std::vector<Eigen::Vector3f>& eigenVec) {
        std::vector<cv::Point3f> cvVec;
        cvVec.reserve(eigenVec.size());
    
        for (const auto& vec : eigenVec) {
            cvVec.emplace_back(vec.x(), vec.y(), vec.z());
        }
    
        return cvVec;
    }

} // namespace ProfileScanner
