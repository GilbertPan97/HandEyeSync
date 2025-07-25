#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include "common.h"

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <Eigen/Core>

class DataProc {
private:
    CalibObj calib_obj_;                            // calibration object
    std::vector<std::vector<cv::Point3f>> scan_lines_;          // original scan lines data

    // if process object is: sphere
    std::vector<cv::Point3f> ctr_pnts_;         // sphere center points (camera frame)

    // if process object is: block
    std::vector<cv::Point3f> block_edge_;       // block edge line points (camera frame)

    // if process object is: calibration board
    std::vector<cv::Point3f> tri_edge0_;        // triangle edge 0 (left side)
    std::vector<cv::Point3f> tri_edge1_;        // triangle edge 1 (right side)

public:
    std::vector<std::vector<cv::Point3f>> filtered_lines_;      // filtered scan lines data

public:
    DataProc();

    DataProc(std::vector<std::vector<cv::Point3f>> scan_lines, CalibObj obj);

    void SetScanData(std::vector<std::vector<cv::Point3f>> scan_lines, CalibObj obj);

    bool SetDataFilter(float eps_noise, int minPts_noise, float rad_backg, int minPts_backg);

    bool SetSingleDataFilter(size_t dataIndex, float eps_noise, int minPts_noise, float rad_backg, int minPts_backg);

    std::vector<cv::Point3f> CalcSphereCtrs(float rad_sphere, std::string dirY_sCtrInCam = "+Y");          // used for calib obj: sphere

    std::vector<std::vector<cv::Point3f>> CalcTriEdgePntsInCamera();    // used for calib obj: calib board

    std::vector<std::vector<cv::Point3f>> CalcTriEdgePntsInRobase(Eigen::Vector<float, 6> vec_pose_board, Eigen::Vector3f tri_vert, float tri_angle);

    std::vector<cv::Point3f> CalcBlockEdgePnts(int id_corner, float tolerance);

private:
    std::vector<cv::Point3f> FilterBackground(std::vector<cv::Point3f> filter_line, float eps, int minPts);

    void dbscan(const std::vector<cv::Point3f>& points, float eps, int minPts, std::vector<int>& labels);

    std::vector<cv::Point3f> getLargestCluster(const std::vector<std::vector<cv::Point3f>>& clusters);

    bool is_circle_PntCloud(std::vector<cv::Point3f> cluster, cv::Point3f center, float rmse_thresh);

    std::vector<cv::Point3f> radiusOutlierRemoval(const std::vector<cv::Point3f>& points, int minPts, float radius);

    void fitCircle(const std::vector<cv::Point2f>& points, cv::Point2f& center, float& radius, bool display = false);

    void fit_display(const std::vector<cv::Point2f> points, cv::Point2f ctr_circle, float rad_circle);

    std::vector<cv::Point3f> getCornerPoints(const std::vector<cv::Point3f>& polyline, float tolerance);

    std::vector<cv::Point3f> to3D(std::vector<cv::Point2f> points_2d);

    std::vector<cv::Point2f> to2D(std::vector<cv::Point3f> point_3d);
};

#endif