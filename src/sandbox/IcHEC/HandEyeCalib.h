// HandEyeCalib.h

#ifndef HANDEYECALIB_H
#define HANDEYECALIB_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <math.h>
#include <Eigen/CORE>
#include <Eigen/Geometry>
#include <Eigen/StdVector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>

enum BoardType {
	CHESS_BOARD = 0,
	CIRCLES_BOARD = 1,
	ASSYMETRIC_CIRCLES = 2
};

enum CalibType {
	EYE_TO_HAND = 0,
	EYE_IN_HAND = 1
};

struct CalibBoard {
	int rows;
	int cols;
	float space;
	BoardType type;
};

class HandEyeCalib
{
private:
	// initial parameters
	Eigen::Matrix3f camintr_ = Eigen::Matrix3f::Zero();     // camera intrinsic parameters
	//Eigen::VectorXf camdist_;								// camera distortion parameters
	CalibType method_;
	CalibBoard board_;

	// calibrate datasets
	std::vector<int> idx_reject_;							// The index of reject datas
	std::vector<Eigen::Vector<float, 6>> vec_end2base_;		// Robot end flange pose (in robot base frame), XYZWPR format. unit: mm & degree
	std::vector<Eigen::Matrix4f> mtr_end2base_;

	std::vector<cv::Mat> imgs_rgb_;							// RGB original images. Mat: RGB three channel.
	std::vector<Eigen::Vector<float, 6>> vec_board2cam_;	// Checkerboard pose (in camera frame), XYZWPR format. unit: mm & degree
	std::vector<Eigen::Matrix4f> mtr_board2cam_;

	// Results
	Eigen::Vector<float, 6> vec_cam2hand_;					// Pose of camera frame relative to robot base frame, XYZWPR format. unit: mm & degree
	Eigen::Matrix4f mtr_cam2hand_;
	std::vector<cv::Mat> imgs_rgb_features_;				// Checkerboard  RGB images with feature point markers. Mat: RGB three channel.
	std::vector<cv::Point2f> imagesCornerPoint_;			// Images corner pixel coordinates (n images).

public:
	HandEyeCalib();

	/* @brief Construct hand eye calibration object.
	 * @param camIntr (Matrix3f): camera intrinsic parameters
	 * @param method (CalibType): calibrate method -- EYE_TO_HAND or EYE_IN_HAND (namespace: RobCoorCalib)
	 */
	HandEyeCalib(const Eigen::Matrix3f camIntr, CalibType method);
	~HandEyeCalib();

	bool SetCamIntr(Eigen::Matrix3f camintr);

	Eigen::Matrix3f GetCamIntr();

	bool SetCalibType(CalibType method);

	/* @brief Set calibration board to hand eye calibration object.
	 * @param board (CalibBoard): Calibrate board object, include rows, cols, space and type (namespace: RobCoorCalib) of board.
	 * @return bool: The status of set calibrate board
	 */
	bool SetCalibBoard(CalibBoard board);

	/* @brief Set calibration board to hand eye calibration object.
	 * @param cols (int): Board cols - Corner number of columes
	 * @param rows (int): Board rows - Corner number of rows
	 * @param space (float): Board space - Size of calibrate board grid
	 * @param type (BoardType): Board type - CHESS_BOARD or CIRCLES_BOARD or ASSYMETRIC_CIRCLES (namespace: RobCoorCalib).
	 * @return bool: The status of set calibrate board
	 */
	bool SetCalibBoard(const int cols, const int rows, const float space, BoardType type);

	/* @brief Set images datas to hand eye calibration object.
	 * @param imgs_rgb (vector<Mat>): RGB images datasets
	 * @param pnt_cloud (vector<Mat>): Point cloud datasets
	 * @return bool: The status of set images datasets
	 */
	bool SetImages(const std::vector<cv::Mat> imgs_rgb);


	/* @brief Set robot pose datas to hand eye calibration object.
	 * @param robPose (vector<sfr::Pose<float>>): Robot pose data, defined as position (XYZ) and orientation (quaternion: wxyz).
	 * @return bool: The status of set robot pose datasets
	 */
	bool SetRobPose(const std::vector<std::vector<float>> xyzwpr);

	/* @brief Get board feature points (pixel coordinates).
	 * @param img_rgb (cv::Mat): Input RGB image to calculate feature point pixel coordinate.
	 * @param img_pnts (vector<sfr::Point2D<float>>&): Output the pixel coordinate of RGB image feature point.
	 * @return bool: The status of get board feature points.
	 */
	bool GetBoardFeaturePoints(const cv::Mat img_rgb, std::vector<cv::Point2f>& img_pnts_uv);

	/* @brief Get the pose of board frame in camera frame
	 * @return std::vector<Eigen::Vector<float, 6>>: The list of pose vector (xyzwpr format: mm & degree).
	 */
	std::vector<Eigen::Vector<float, 6>> GetBoardToCamVec();


	/* @brief Calibration execution for object.
	 * @param matrix_cam2rob(Matrix4f&): Output result, HTM of camera to robot
	 * @return bool: The status of calibration
	 */
	bool CalcResult(Eigen::Matrix4f& matrix_cam2rob);

	/* @brief Calculate calibration error.
	 * @param rota_error (float&): Output result, average algorithm error of rotational component
	 * @param trans_error (float&): Output result, average algorithm error of translational component
	 * @return bool: The status of calculate error
	 */
	bool CalcError(float& rota_error, float& trans_error);

	/* @brief Calculate calibration board check point (Method: PnP).
	 * @param rgb_image_in (Mat): Input RGB image for calculate check point position.
	 * @param check_board (CalibBoard): Input check board struct.
	 * @param coor_UV_out (sfr::Point2D<float>&): Output pixel coordinate of cherk point in RGB image.
	 * @param  coor_incam_out (sfr::Point<float>&): Output XYZ coordinate of cherk point in camera frame.
	 * @return bool: The status of check point calculation.
	 */
	bool CalcCheckPoint(const cv::Mat rgb_image_in, cv::Point2f& coor_uv_out, cv::Point3f& coor_incam_out);

	/* ------------------Test interface----------------------- */
	bool SetRobPose(std::vector<Eigen::Vector<float, 6>> robPose);
	Eigen::Vector<float, 6> GetCamToRobVec();
	Eigen::Matrix4f GetCamToRobMtr();
	std::vector<Eigen::Vector<float, 6>> GetRobPose();
	std::vector<cv::Mat> GetBoardImgWirhFeatures();

private:
	bool FindBoardFeaturePoints(cv::Mat image, CalibBoard caliboard, std::vector<cv::Point2f>& imgPoints);
	cv::Mat DrawBoardFeaturePoints(cv::Mat image, CalibBoard caliboard, std::vector<cv::Point2f> imgPoints);

	Eigen::Matrix4f RT2HomogeneousMatrix(const Eigen::Matrix3f& R, const Eigen::Vector3f& T);
	void HomogeneousMtr2RT(const Eigen::Matrix4f& homoMtr, Eigen::Matrix3f& R, Eigen::Vector3f& T);
	Eigen::Matrix4f XYZWPRVecToHTM(const Eigen::VectorXf XYZWPR);
	Eigen::VectorXf HTMToXYZWPRVec(const Eigen::Matrix4f HTM);

	cv::Mat toCvMat(const Eigen::MatrixXf& mtr_eigen);
	Eigen::MatrixXf toEigenMatrix(const cv::Mat& mtr_cv);
	Eigen::VectorXf toEigenVector(const cv::Mat& vec_cv);

	bool isRotatedMatrix(const Eigen::Matrix3f& R);
	bool isInVector(const int idx, std::vector<int> idxVector);

	Eigen::Matrix4f CalBoardToCameraMatrix(std::vector<cv::Point2f> imgPoints, Eigen::Matrix3f camintr, Eigen::VectorXf camdist = Eigen::VectorXf::Zero(12));
	// Eigen::Matrix4f CalBoardToCameraMatrix(const std::vector<cv::Point2f>& PointBuf, const cv::Mat& pcdImg, const Eigen::Matrix3f& camintr, const CalibBoard caliboard);
	cv::Point3f CalcXYZByGazeline(const Eigen::Matrix3f& camIntr, float& PA, float& PB, float& PC, float& PD, const cv::Point2f& uv);
};

#endif 	// HANDEYECALIB_H