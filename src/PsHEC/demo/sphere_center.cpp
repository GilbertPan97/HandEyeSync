/*
 * ============================================================
 *  Sphere Fitting from PLY Point Cloud using PCL
 * ============================================================
 *
 *  This program loads a PLY point cloud file and fits a sphere
 *  using RANSAC. The output will be the sphere center (x, y, z)
 *  and the estimated radius.
 *
 *  ------------------------------------------------------------
 *  Usage Guide:
 *  ------------------------------------------------------------
 *  1. Build the project with CMake:
 *
 *     mkdir build && cd build
 *     cmake ..
 *     make -j4
 *
 *  2. Run the executable with a PLY file:
 *
 *     ./fit_sphere ../../data/SphereCloud.ply mm
 *
 *  3. Example output:
 *
 *     Loaded 12567 points from ../SphereCloud.ply
 *     Sphere center: (47.9842, 6.42687, 455.605)
 *     Sphere radius: 40.0868
 *
 *  ------------------------------------------------------------
 *  Requirements:
 *  ------------------------------------------------------------
 *  - PCL >= 1.8
 *  - Eigen (comes with PCL)
 *  - C++14 or higher
 *
 *  ------------------------------------------------------------
 *  Author: PanJiabin
 *  Date:   2025-09-23
 * ============================================================
 */

#include <iostream>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_sphere.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ply> [unit]" << std::endl;
        std::cerr << "unit: mm (default), cm, m" << std::endl;
        return -1;
    }

    std::string plyFile = argv[1];
    std::string unit = (argc >= 3) ? argv[2] : "mm";

    float scale = 1000.0f; // default mm
    if (unit == "m") scale = 1.0f;
    else if (unit == "cm") scale = 100.0f;
    else if (unit == "mm") scale = 1000.0f;
    else {
        std::cerr << "Unknown unit '" << unit << "'. Using mm by default." << std::endl;
        scale = 1000.0f;
    }

    // Define PointCloud type
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    // Load PLY file
    if (pcl::io::loadPLYFile<pcl::PointXYZ>(argv[1], *cloud) == -1) {
        PCL_ERROR("Couldn't read PLY file.\n");
        return -1;
    }
    std::cout << "Loaded " << cloud->points.size() << " points from " << argv[1] << std::endl;

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
    ransac.setDistanceThreshold(0.01); // adjust according to your point cloud scale
    ransac.computeModel();

    Eigen::VectorXf coeff;
    ransac.getModelCoefficients(coeff);

    if (coeff.size() == 4) {
        std::cout << "Sphere center: ("
                  << coeff[0] << ", "
                  << coeff[1] << ", "
                  << coeff[2] << ")" << std::endl;
        std::cout << "Sphere radius: " << coeff[3] << std::endl;
    } else {
        std::cerr << "Sphere fitting failed!" << std::endl;
    }

    return 0;
}
