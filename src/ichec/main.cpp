#include "HandEyeCalib.h"

using namespace cv;
using namespace Eigen;
using namespace std;

std::vector<Vector<float, 6>> GenRobPose();
int main() {

	// 1.Initial camera parameters
	Matrix3f camintr;
	camintr << 1007.7047119140625, 0, 679.7445068359375,
				0, 1007.63330078125, 474.7272644042969,
				0, 0, 1;

	// 2.Construct a hand-eye calibration object
	HandEyeCalib HEC(camintr, EYE_TO_HAND);		//constructor (camera intrinsics parameters, calibration type)

	// 3.Set the calibration board
	CalibBoard board{ 8, 11, 30.0, CHESS_BOARD };
	HEC.SetCalibBoard(board);

	// 4.Robot calibration position setting
	vector<Vector<float, 6>> robPos = GenRobPose();
	HEC.SetRobPose(robPos);

	// 5.Corresponding RGB & point cloud images settings
	cout << "Reading calibration datas..." << std::endl;
	string imgs_path = "../../data/calibration";
	vector<Mat>rgb_images;				// RGB & point cloud images
	for (int i = 0; i < HEC.GetRobPose().size(); i++) {
        // Construct the image file path
        std::string imagePath = imgs_path + "/rgb/" + std::to_string(i) + "_color.png";

        // Attempt to read the image
        cv::Mat image = cv::imread(imagePath);

        // Check if the image was loaded successfully
        if (image.empty()) {
            // Print error message if the image failed to load
            std::cerr << "Error: Failed to load image at " << imagePath << std::endl;
        } else {
            // If image is loaded successfully, push it to the rgb_images vector
            rgb_images.push_back(image);
        }
	}
	HEC.SetImages(rgb_images);			// Set 2D image dataset

	// 6.Hand-eye calibration execution
	cout << "Calibration execution..." << std::endl;
	Matrix4f matrix_cam2rob;
	bool state = HEC.CalcResult(matrix_cam2rob);		// Calibration execution

	// 7.Get calibration results (Pose of the camera relative to the robot base)
	cout << "The calibration result is (Pose of the camera relative to the robot base):" << std::endl;
	VectorXf camToBase_vec = HEC.GetCamToRobVec();		// XYZWPR
	Matrix4f camToBase_mtr = HEC.GetCamToRobMtr();		// 4*4 Homogenous transformation matrix
	cout << "Matrix form:\n" << camToBase_mtr << std::endl;
	cout << "XYZWPR form:" << camToBase_vec.transpose() << std::endl;

	// Get calibration board feature images (optional)
	vector<Mat> board_features = HEC.GetBoardImgWirhFeatures();
	for (int i = 0; i < board_features.size();i++) {
		cv::imshow("feature", board_features[i]);
		cv::waitKey(0);
	}

	return 0;
}

std::vector<Vector<float, 6>> GenRobPose()
{
	std::vector<Vector<float, 6>> vecRobPose;

	Eigen::MatrixXf XYZWPR_RobPose(20, 6);
	XYZWPR_RobPose <<
		1100, 0, 600, -162, 18, 0,
		1100, 0, 600, -162, -18, 0,
		1100, 0, 600, -162, 0, 18,
		1100, 0, 600, -162, 0, -18,
		1100, 0, 600, 162, 18, 0,
		1100, 0, 600, 162, 0, 18,
		1100, 0, 600, 162, -18, 0,
		1100, 0, 600, 162, 0, -18,
		1100, 0, 600, -180, 18, 18,
		1100, 0, 600, -180, -18, 18,
		1100, 0, 600, -180, -18, -18,
		1100, 0, 600, -180, 18, -18,
		1100, 0, 600, -162, -18, 18,
		1100, 0, 600, -162, 18, -18,
		1100, 0, 600, -162, -18, -18,
		1100, 0, 600, -162, 18, 18,
		1100, 0, 600, 162, 18, 18,
		1100, 0, 600, 162, -18, 18,
		1100, 0, 600, 162, 18, -18,
		1100, 0, 600, 162, -18, -18;

	for (size_t i = 0; i < XYZWPR_RobPose.rows(); i++){
		vecRobPose.push_back(XYZWPR_RobPose.row(i));
	}
	return vecRobPose;
}