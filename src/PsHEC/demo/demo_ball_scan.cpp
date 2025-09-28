#include "DataProcessor.h"
#include "PsCalibrator.h"
#include "PsAlgorithm.h"
#include "FanucParser.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <tuple>
#include <filesystem>
#include <sstream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_sphere.h>

// Function: Fit sphere from PLY point cloud
// Parameters:
//   plyFile  : path to input PLY file
//   unit     : output unit, "mm", "cm", or "m" (default "mm")
// Returns:
//   tuple of (center_x, center_y, center_z, radius)
//   all values in the requested unit
std::tuple<float, float, float, float> fitSphereFromPLY(const std::string& plyFile,
                                                        const std::string& unit = "mm")
{
    // Determine scale factor for output unit
    float scale = 1000.0f; // default mm
    if (unit == "m") scale = 1.0f;
    else if (unit == "cm") scale = 100.0f;
    else if (unit == "mm") scale = 1000.0f;
    else {
        std::cerr << "[Warning] Unknown unit '" << unit << "', using mm by default." << std::endl;
        scale = 1000.0f;
    }

    // Load PLY file into PointCloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPLYFile<pcl::PointXYZ>(plyFile, *cloud) == -1) {
        throw std::runtime_error("Couldn't read PLY file: " + plyFile);
    }

    if (cloud->empty()) {
        throw std::runtime_error("Loaded point cloud is empty: " + plyFile);
    }

    // Convert units
    for (auto& p : cloud->points) {
        p.x *= scale;
        p.y *= scale;
        p.z *= scale;
    }

    // Sphere model fitting
    pcl::SampleConsensusModelSphere<pcl::PointXYZ>::Ptr model_sphere(
        new pcl::SampleConsensusModelSphere<pcl::PointXYZ>(cloud));

    pcl::RandomSampleConsensus<pcl::PointXYZ> ransac(model_sphere);
    ransac.setDistanceThreshold(0.01f * scale); // adjust according to scale
    ransac.computeModel();

    Eigen::VectorXf coeff;
    ransac.getModelCoefficients(coeff);

    if (coeff.size() != 4) {
        throw std::runtime_error("Sphere fitting failed!");
    }

    return std::make_tuple(coeff[0], coeff[1], coeff[2], coeff[3]);
}

// Convert std::vector<std::vector<float>> to std::vector<Eigen::Matrix<float, 6, 1>>
std::vector<Eigen::Matrix<float, 6, 1>> convertToEigenVector6(
    const std::vector<std::vector<double>>& input) 
{
    std::vector<Eigen::Matrix<float, 6, 1>> output;
    output.reserve(input.size());

    for (const auto& vec : input) {
        if (vec.size() != 6) {
            throw std::runtime_error("Each inner vector must have exactly 6 elements.");
        }

        Eigen::Matrix<float, 6, 1> eigenVec;
        for (int i = 0; i < 6; ++i) {
            eigenVec(i) = vec[i];
        }
        output.push_back(eigenVec);
    }

    return output;
}

// Read sphere centers from TXT file (each line: X Y Z)
std::vector<cv::Point3f> readCentersFromTXT(const std::string& filePath) {
    std::vector<cv::Point3f> centers;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return centers;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        float x, y, z;
        ss >> x >> y >> z;
        if (!ss.fail()) {
            centers.emplace_back(x, -y, z);
        }
    }

    return centers;
}

std::string datasetDir = "../../data/Projects/HuaLiang-09.23/R1/";

int main(int argc, char* argv[])
{
	// 1. Read center from TXT file or parser from ply data
	int snap_cnt = 15;
	std::string file_path;
    std::vector<cv::Point3f> ctr_pnts;
    std::vector<float> radius;

    std::string centers_file_path = datasetDir + "SphereCenters1.txt";
    if (std::filesystem::exists(centers_file_path)) {
        ctr_pnts = readCentersFromTXT(centers_file_path);
    } 
    else {
        for (size_t i = 0; i < snap_cnt; i++){
            // read from ply file and parser center
            file_path = datasetDir + "p" + std::to_string(i + 1) +".ply";
            try {
                auto [cx, cy, cz, r] = fitSphereFromPLY(file_path, "mm");
                ctr_pnts.push_back(cv::Point3f(cx, -cy, cz));
                radius.push_back(r);
            }
            catch (const std::exception& e) {
                std::cerr << "Error processing " << file_path << ": " << e.what() << std::endl;
            }
        }
    }

	// 2. Parse the file using the FanucParser
	std::string file_path1;
	file_path1 = datasetDir + "calib_sph.ls";

	FanucParser parser;
    std::vector<std::vector<double>> vec_rob_pose;
	vec_rob_pose = parser.parseRobFile(file_path1, "LS");

	// 3. Create calibration 
	ProfileScanner::HandEyeCalib hec;

	hec.SetCalibType(CalibType::EYE_IN_HAND);
	hec.SetRobPose(convertToEigenVector6(vec_rob_pose));
	hec.SetProfileData(ctr_pnts, CalibObj::SPHERE);

	hec.run(ProfileScanner::SolveMethod::ITERATION);

    Eigen::Vector<float, 6> xyzwpr = hec.GetCalcResultVec();

    std::cout << "=== Hand-Eye Calibration Result ===" << std::endl;
    std::cout << "X: " << xyzwpr(0) << " mm" << std::endl;
    std::cout << "Y: " << xyzwpr(1) << " mm" << std::endl;
    std::cout << "Z: " << xyzwpr(2) << " mm" << std::endl;
    std::cout << "Rx: " << xyzwpr(3) << " deg" << std::endl;
    std::cout << "Ry: " << xyzwpr(4) << " deg" << std::endl;
    std::cout << "Rz: " << xyzwpr(5) << " deg" << std::endl;

    // 4. Calculate calibration result
    auto err = hec.CalcCalibError("Sphere");
    std::cout << "Calibration error (RMS): " << err << " mm" << std::endl;

	return 0;
}