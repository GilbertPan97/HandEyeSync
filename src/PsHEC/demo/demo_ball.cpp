#include "DataProcessor.h"
#include "PsCalibrator.h"
#include "PsAlgorithm.h"
#include "FanucParser.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <Eigen/Core>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkAxesActor.h>
#include <vtkNamedColors.h>

using namespace std;

void viewer_3D(std::vector<cv::Point3f> pnt_cloud);
std::vector<Eigen::Matrix<float, 6, 1>> convertToEigenVector6(
    const std::vector<std::vector<double>>& input);

std::string datasetDir = "../../data/Experiments/Sphere/";


int main(int argc, char* argv[])
{
	// Read scan lines data
	int snap_cnt = 15;
	std::string file_path;
	std::vector<std::vector<cv::Point3f>> scan_lines;
	for (size_t i = 0; i < snap_cnt; i++){
		// read from yml file
		file_path = datasetDir + "p" + std::to_string(i + 1) +".yml";

		std::vector<cv::Point3f> scan_line;
		cv::FileStorage fs(file_path, cv::FileStorage::READ);
		fs["profile"] >> scan_line;

		scan_lines.push_back(scan_line);
		fs.release();

		// check view
        viewer_3D(scan_line);
	}

	// process scan data
	DataProc proc(scan_lines, CalibObj::SPHERE);
	float rad_sphere = 80 / 2.0;
	std::vector<cv::Point3f> ctr_pnts = proc.CalcSphereCtrs(rad_sphere, "-Y");

	// 4. Parse the file using the FanucParser
	std::string file_path1;
	file_path1 = datasetDir + "calib_sph.ls";

	FanucParser parser;
    std::vector<std::vector<double>> vec_rob_pose;
	vec_rob_pose = parser.parseRobFile(file_path1, "LS");

	// Create calibration 
	ProfileScanner::HandEyeCalib hec;

	hec.SetCalibType(CalibType::EYE_IN_HAND);
	hec.SetRobPose(convertToEigenVector6(vec_rob_pose));
	hec.SetProfileData(ctr_pnts, CalibObj::SPHERE);

	hec.run(ProfileScanner::SolveMethod::ITERATION);

	auto err = hec.CalcCalibError("Sphere");

	return 0;
}

void viewer_3D(std::vector<cv::Point3f> pnt_cloud) {
    // Create points
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (const auto& p : pnt_cloud) {
        points->InsertNextPoint(p.x, p.y, p.z);
    }

    // Create a polydata to store points
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);

    // Use glyph filter to render points as vertices
    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
        vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputData(polyData);
    glyphFilter->Update();

    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyphFilter->GetOutputPort());

    // Actor
    vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.0, 1.0, 0.0);  // Green
    actor->GetProperty()->SetPointSize(3);          // Point size

    // Coordinate system axes
    vtkSmartPointer<vtkAxesActor> axes =
        vtkSmartPointer<vtkAxesActor>::New();
    axes->SetTotalLength(10.0, 10.0, 10.0);

    // Renderer
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->AddActor(axes);
    renderer->SetBackground(0.1, 0.1, 0.1);  // Dark background

    // Render window
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(800, 600);
    renderWindow->SetWindowName("Point Cloud");

    // Interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();

    // Start interaction
    renderWindow->Render();
    interactor->Start();
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
