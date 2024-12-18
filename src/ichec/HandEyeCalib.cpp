#include "HandEyeCalib.h"

#include <math.h>
#include <numeric>

using namespace std;
using namespace cv;
using namespace Eigen;

HandEyeCalib::HandEyeCalib() {

}

HandEyeCalib::HandEyeCalib(const Eigen::Matrix3f camIntr, CalibType method) {
	camintr_ = camIntr;
	method_ = method;
}

HandEyeCalib::~HandEyeCalib()
{
	
}

bool HandEyeCalib::SetCamIntr(Eigen::Matrix3f camintr) {
	camintr_ = camintr;
	return true;
}

Eigen::Matrix3f HandEyeCalib::GetCamIntr() {
	return camintr_;
}

bool HandEyeCalib::SetCalibType(CalibType  method) {
	method_ = method;
	return true;
}

bool HandEyeCalib::SetCalibBoard(CalibBoard board)
{
	board_ = board;
	return true;
}

bool HandEyeCalib::SetCalibBoard(const int cols, const int rows, const float space, BoardType type)
{
	board_.cols = cols;
	board_.rows = rows;
	board_.space = space;
	board_.type = type;
	return true;
}

bool HandEyeCalib::SetImages(const std::vector<cv::Mat> imgs_rgb) {
	if (imgs_rgb.empty()) {
		// TODO: Throw error if imgs_rgb data is empty.
		std::cerr << "RGB images set fail.\n";
		return false;
	}
	else {
		imgs_rgb_ = imgs_rgb;
		mtr_board2cam_.clear();		// clear object data before set camera images
	}

	// Calculate board to camera HTM
	vector<cv::Point2f> imgFeaturePnts;		// Pixel coordinates of feature points in one image
	Matrix4f HTM_temp;
	VectorXf xyzwpr_tmp(6);
	for (size_t i = 0; i < imgs_rgb_.size(); i++) {

		Mat img_tmp = imgs_rgb_[i].clone();
		bool findSta = FindBoardFeaturePoints(img_tmp, board_, imgFeaturePnts);

		if (findSta == true) {
			imgs_rgb_features_.push_back(DrawBoardFeaturePoints(img_tmp, board_, imgFeaturePnts));
			HTM_temp = CalBoardToCameraMatrix(imgFeaturePnts, camintr_);
			xyzwpr_tmp = HTMToXYZWPRVec(HTM_temp);
			std::cout << "INFO: Find chess board-[" << i << "] feature point success!\n";
		}
		else {
			imgs_rgb_features_.push_back(img_tmp);		// if feature points not found, imgs_rgb_features_[i] save as origin image 
			HTM_temp.setZero();				// if images rejected, mtr_board2cam_[i] set as zeros
			std::cout << "The chess board-[" << i << "] feature point was not found.\n";
			idx_reject_.push_back(i);
		}

		mtr_board2cam_.push_back(HTM_temp);
		vec_board2cam_.push_back(xyzwpr_tmp);
	}
	return true;
}

bool HandEyeCalib::SetRobPose(const std::vector<std::vector<float>> xyzwpr)
{
	if (xyzwpr.empty()) {
		std::cerr << " Input data (robot pose) was empty.\n";
		return false;
	} else {
		vec_end2base_.clear();		// clear object data before set robot pose
		mtr_end2base_.clear();
	}

	// Conversion loop: Convert each std::vector<float> to Eigen::Vector<float, 6>
    for (const auto& item : xyzwpr) {
        if (item.size() == 6) {
            // Create an Eigen::Vector<float, 6> and fill it with data from the std::vector<float>
            Eigen::Vector<float, 6> vec;
            for (int i = 0; i < 6; ++i) {
                vec[i] = item[i];  // Assign each element from the vector to the Eigen vector
            }
            vec_end2base_.push_back(vec);  // Add the Eigen vector to the result vector
        } else {
            // Error handling: Check if the input vector has exactly 6 elements
            std::cerr << "Error: Expected 6 elements in the vector, but got " << item.size() << std::endl;
        }
    }

	for (int i = 0; i < vec_end2base_.size(); i++) {
		Matrix4f HTMend2base;
		HTMend2base = XYZWPRVecToHTM(vec_end2base_[i]);
		HandEyeCalib::mtr_end2base_.push_back(HTMend2base);
	}
	return true;
}

bool HandEyeCalib::GetBoardFeaturePoints(const cv::Mat img_rgb, std::vector<cv::Point2f>& img_pnts)
{
	//	check board
	if (board_.type != CHESS_BOARD && board_.type != CIRCLES_BOARD
		&& board_.type != ASSYMETRIC_CIRCLES) {
		std::cerr << "The board is not defined before get board feature points.\n";
		return false;
	}

	cv::Size nbrBoardPart(board_.cols, board_.rows);
	float partSize = board_.space;			// size of calibration board grid

	bool check = findChessboardCorners(img_rgb, nbrBoardPart, img_pnts, cv::CALIB_CB_ADAPTIVE_THRESH);
	if (check == false) {
		std::cerr << "Find calibration board feature point fail.\n";
		return false;
	}

	return true;
}

std::vector<Eigen::Vector<float, 6>> HandEyeCalib::GetBoardToCamVec() {
	if (vec_board2cam_.empty()) {
		std::cerr << "Board to camera vector list is empty.\n";
		exit(-1);
	}
	return vec_board2cam_;
}

bool HandEyeCalib::CalcResult(Eigen::Matrix4f& matrix_cam2rob)
{
	//	check board
	if (board_.type != CHESS_BOARD && board_.type != CIRCLES_BOARD
		&& board_.type != ASSYMETRIC_CIRCLES) {
		std::cerr << "The board is not defined before calibration execution.\n";
		return false;
	}

	Matrix3f R_temp;	
	Vector3f T_temp;		// Temporary data in the loops

	// calculate the pose of target(board) in camera frame
	vector<cv::Mat> R_target2cam;
	vector<cv::Mat> T_target2cam;
	for (size_t i = 0; i < mtr_board2cam_.size(); i++) {
		// Query whether the data exists in the rejected data index
		if (isInVector(i, idx_reject_))
			continue;		// skip when reading the rejected calibration data
		else {
			HomogeneousMtr2RT(mtr_board2cam_[i], R_temp, T_temp);
			R_target2cam.push_back(toCvMat(R_temp));
			T_target2cam.push_back(toCvMat(T_temp));
		}
	}

	// calculate the pose of gripper(robot end flange) in robot base frame
	vector<cv::Mat> R_gripper2base;
	vector<cv::Mat> T_gripper2base;
	for (size_t i = 0; i < mtr_end2base_.size(); i++) {
		if (isInVector(i, idx_reject_))
			continue;
		else {
			HomogeneousMtr2RT(mtr_end2base_[i], R_temp, T_temp);
			R_gripper2base.push_back(toCvMat(R_temp));
			T_gripper2base.push_back(toCvMat(T_temp));
		}
	}

	// calibration execution (calculate the pose of camera in robot base frame)
	Mat R_cam2hand = Mat(3, 3, CV_32F);
	Mat T_cam2hand = Mat(3, 1, CV_32F);
	if (method_ == EYE_IN_HAND) {
		// TODO: Verify eye in hand calibration code
		cv::calibrateHandEye(R_gripper2base, T_gripper2base, R_target2cam, T_target2cam, R_cam2hand, T_cam2hand, cv::CALIB_HAND_EYE_TSAI);
	}
	else if (method_ == EYE_TO_HAND) {
		vector<cv::Mat> R_base2gripper;
		vector<cv::Mat> T_base2gripper;
		Matrix4f HTM_base2gripper;
		for (size_t i = 0; i < R_gripper2base.size(); i++)
		{
			HTM_base2gripper = RT2HomogeneousMatrix(toEigenMatrix(R_gripper2base[i]), toEigenMatrix(T_gripper2base[i])).inverse();
			HomogeneousMtr2RT(HTM_base2gripper, R_temp, T_temp);
			R_base2gripper.push_back(toCvMat(R_temp));
			T_base2gripper.push_back(toCvMat(T_temp));
		}
		cv::calibrateHandEye(R_base2gripper, T_base2gripper, R_target2cam, T_target2cam, R_cam2hand, T_cam2hand, cv::CALIB_HAND_EYE_TSAI);
	}
	else {
		std::cerr << "The calibration type is not defined.\n";
	}
	
	// convert calibration results
	mtr_cam2hand_ = RT2HomogeneousMatrix(toEigenMatrix(R_cam2hand), toEigenVector(T_cam2hand));

	Matrix3f R_cam2hand_eigen = toEigenMatrix(R_cam2hand);
	Vector3f RPW_cam2hand = R_cam2hand_eigen.eulerAngles(2,1,0) * 180/M_PI;		// RPW_cam2hand unit: degree
	Vector3f XYZ_cam2hand = toEigenVector(T_cam2hand);
	vec_cam2hand_ << XYZ_cam2hand, RPW_cam2hand(2), RPW_cam2hand(1), RPW_cam2hand(0);

	matrix_cam2rob = mtr_cam2hand_;

	return 1;
}

bool HandEyeCalib::CalcError(float& rota_error, float& trans_error)
{
	if (mtr_end2base_.empty()){
		std::cerr << " Please set robot pose data.\n";
		return false;
	}
	else if (mtr_board2cam_.empty()){
		std::cerr << " Please set images/point_cloud data.\n";
		return false;
	}

	// Reconstruct accepted data
	vector<Eigen::Matrix4f> mtr_end2base_acc;
	vector<Eigen::Matrix4f> mtr_board2cam_acc;
	for (size_t i = 1; i < mtr_end2base_.size(); i++)
	{
		if (isInVector(i, idx_reject_))
			continue;
		else {
			mtr_end2base_acc.push_back(mtr_end2base_[i]);
			mtr_board2cam_acc.push_back(mtr_board2cam_[i]);
		}
	}

	vector<float> r_errs, t_errs;
	Matrix4f tmpH_ax, tmpH_xb;			// Temporary data in the loops (HTM of AX and XB)
	Matrix3f tmpR_ax, tmpR_xb;			// AX=XB, Rotational component of AX and XB
	Vector3f tmpT_ax, tmpT_xb;			// AX=XB, Translational component of AX and XB
	for (size_t i = 0; i < mtr_end2base_acc.size()-1; i++)
	{
		if (method_ == EYE_IN_HAND) {
			tmpH_ax = mtr_end2base_acc[i].inverse() * mtr_end2base_acc[i + 1] * mtr_cam2hand_;
			tmpH_xb = mtr_cam2hand_ * mtr_board2cam_acc[i] * (mtr_board2cam_acc[i + 1].inverse());
		}
		else if (method_ == EYE_TO_HAND) {
			tmpH_ax = mtr_end2base_acc[i] * (mtr_end2base_acc[i + 1].inverse()) * mtr_cam2hand_;
			tmpH_xb = mtr_cam2hand_ * mtr_board2cam_acc[i] * (mtr_board2cam_acc[i + 1].inverse());
		}
		else {
			std::cerr << "The calibration type was not defined.\n";
			return false;
		}
		HomogeneousMtr2RT(tmpH_ax, tmpR_ax, tmpT_ax);
		HomogeneousMtr2RT(tmpH_xb, tmpR_xb, tmpT_xb);

		r_errs.push_back( abs(acos(((tmpR_ax.transpose() * tmpR_xb).trace() - 1) / 2)) * 180/M_PI );
		t_errs.push_back((tmpT_ax - tmpT_xb).norm());
	}

	rota_error = std::accumulate(r_errs.begin(), r_errs.end(), 0.0) / r_errs.size();
	trans_error = std::accumulate(t_errs.begin(), t_errs.end(), 0.0) / t_errs.size();

	return true;
}

bool HandEyeCalib::CalcCheckPoint(const cv::Mat rgb_image_in, cv::Point2f& coor_uv_out, cv::Point3f& coor_incam_out)
{
	cv::Size nbrBoardPart(board_.cols, board_.rows);
	float partSize = board_.space;			// size of calibration board grid

	vector<cv::Point2f> imagePoints;	// Pixel coordinate of feature points in the board image
	bool check = findChessboardCorners(rgb_image_in, nbrBoardPart, imagePoints, cv::CALIB_CB_ADAPTIVE_THRESH);
	if (check == false) {
		std::cerr << "Find calibration board feature point fail.\n";
		return false;
	}
	else {
		// Board frame origin is set as the check point
		coor_uv_out = imagePoints[0];	// Output origin pixel coordinate.
	}

	// Calculate corner point (board origin) coordinate
	vector<Point3f> visualFeaturePointSet;
	Point3f visualFeaturePoint;
	for (size_t i = 0; i < nbrBoardPart.height; i++)
	{
		for (size_t j = 0; j < nbrBoardPart.width; j++)
		{
			// define board frame the same as cv::findChessboardCorners
			visualFeaturePoint.x = j * partSize;	// X-direction is set as width direction
			visualFeaturePoint.y = i * partSize;	// Y-direction is set as height direction
			visualFeaturePoint.z = 0;
			visualFeaturePointSet.push_back(visualFeaturePoint);
		}
	}

	Point3f FeaturePnt1;
	vector<Point3f> FeaturePntSet1;
	Mat Rvec_board2cam, Rmtr_board2cam, Tvec_board2cam;
	Mat CamIntr, CamDist;
	eigen2cv(camintr_, CamIntr);
	CamDist.zeros(12, 1, CV_32F);		// Input image is de-distorted, CamDist set as zeros
	bool sta = solvePnP(visualFeaturePointSet, imagePoints, CamIntr, CamDist, Rvec_board2cam, Tvec_board2cam, false, cv::SOLVEPNP_ITERATIVE);
	Rodrigues(Rvec_board2cam, Rmtr_board2cam);

	coor_incam_out.x = Tvec_board2cam.at<float>(0, 0);
	coor_incam_out.y = Tvec_board2cam.at<float>(1, 0);
	coor_incam_out.z = Tvec_board2cam.at<float>(2, 0);

	return true;
}

/* ------------------------------- Test interface --------------------------------- */
bool HandEyeCalib::SetRobPose(std::vector<Eigen::Vector<float, 6>> robPose)
{
	if (robPose.empty()) {
		std::cerr << "Input robot data was empty.\n";
		return false;
	}
	else {
		mtr_end2base_.clear();
	}

	vec_end2base_ = robPose;
	for (int i = 0; i < vec_end2base_.size(); i++) {
		Matrix4f HTMend2base;
		HTMend2base = XYZWPRVecToHTM(vec_end2base_[i]);
		mtr_end2base_.push_back(HTMend2base);
	}

	return true;
}

Eigen::Vector<float, 6> HandEyeCalib::GetCamToRobVec()
{
	return vec_cam2hand_;
}

Eigen::Matrix4f HandEyeCalib::GetCamToRobMtr()
{
	return mtr_cam2hand_;
}

vector<Eigen::Vector<float, 6>> HandEyeCalib::GetRobPose()
{
	return vec_end2base_;
}

vector<cv::Mat> HandEyeCalib::GetBoardImgWirhFeatures()
{
	return imgs_rgb_features_;
}

/* ----------------------------- Private function -------------------------------- */
bool HandEyeCalib::FindBoardFeaturePoints(cv::Mat image, CalibBoard caliboard, vector<cv::Point2f>& imgPoints)
{
	cv::Size boardSize;
	boardSize.width = caliboard.cols;
	boardSize.height = caliboard.rows;

	// Check images and calibrate board datas
	if (image.empty()){
		std::cerr << "Input image is empty.\n";
		return false;
	}

	bool state;
	if (caliboard.type == CHESS_BOARD) {
		state = findChessboardCorners(image, boardSize, imgPoints, cv::CALIB_CB_ADAPTIVE_THRESH);
		if (state) {
			//cout << "find chess calibration board success!" << endl;
			return state;
		}
		else {
			//cout << "can not find calibration board..." << endl;
			return false;
		}
	}
	else if (caliboard.type == CIRCLES_BOARD) {
		state = findCirclesGrid(image, boardSize, imgPoints);
		if (state = 1) {
			//cout << "find circle calibration board success!" << endl;
			return state;
		}
		else {
			//cout << "can not find calibration board..." << endl;
			return false;
		}
	}
	else {
		//cout << "Board type must be CHESS or CIRCLE..." << endl;
		return false;
	}
}

cv::Mat HandEyeCalib::DrawBoardFeaturePoints(cv::Mat image, CalibBoard caliboard, std::vector<cv::Point2f> imgPoints) 
{
	cv::Size boardSize;
	boardSize.width = caliboard.cols;
	boardSize.height = caliboard.rows;

	bool state = 1;
	if (caliboard.type == CHESS_BOARD)
	{
		drawChessboardCorners(image, boardSize, imgPoints, state);
	}
	else if (caliboard.type == CIRCLES_BOARD) {
		
	}else {
		std::cerr << " Draw board fail!\n";
		exit(-1);
	}
	return image;
}

Eigen::Matrix4f HandEyeCalib::RT2HomogeneousMatrix(const Eigen::Matrix3f& R, const Eigen::Vector3f& T)
{
	Matrix4f HomoMtr;
	if (isRotatedMatrix(R) != true) {
		std::cerr << "The matrix is not rotation one.(ERROR:HandEyeCalib::RT2HomogeneousMatrix).\n";
		exit(1);
	}
	else {
		HomoMtr.setIdentity();
		HomoMtr.block(0, 0, 3, 3) = R;
		HomoMtr.block(0, 3, 3, 1) = T;
	}
	return HomoMtr;
}

void HandEyeCalib::HomogeneousMtr2RT(const Eigen::Matrix4f& homoMtr, Eigen::Matrix3f& R, Eigen::Vector3f& T)
{
	R = homoMtr.block(0, 0, 3, 3);
	T = homoMtr.block(0, 3, 3, 1);
}

Eigen::Matrix4f HandEyeCalib::XYZWPRVecToHTM(const Eigen::VectorXf XYZWPR)
{
	if (XYZWPR.rows() != 6){
		std::cerr << "The XYZWPR is not 6-dimention vector (ERROR:Function HandEyeCalib::XYZWPRVecToHTM call).\n";
		exit(1);
	}
	Matrix4f HTM;			// Homogeneous transformation matrix
	Vector3f XYZ = XYZWPR.head(3);
	Vector3f WPR = XYZWPR.tail(3);
	Matrix3f R;
	R = Eigen::AngleAxisf(WPR(2) * M_PI / 180, Vector3f::UnitZ()) *
		Eigen::AngleAxisf(WPR(1) * M_PI / 180, Vector3f::UnitY()) *
		Eigen::AngleAxisf(WPR(0) * M_PI / 180, Vector3f::UnitX());

	HTM.setIdentity();
	HTM.block(0, 0, 3, 3) = R;
	HTM.block(0, 3, 3, 1) = XYZ;

	return HTM;
}

Eigen::VectorXf HandEyeCalib::HTMToXYZWPRVec(const Eigen::Matrix4f HTM) {
	Matrix3f R;
	Vector3f T;
	HomogeneousMtr2RT(HTM, R, T);

	if (!isRotatedMatrix(R)) {
		std::cerr << "HTMToXYZWPRVec input error (not HTM).\n";
		exit(-1);
	}
	Vector3f RPW_euler = R.eulerAngles(2, 1, 0) * 180 / M_PI;	// degree

	VectorXf xyzwpr(6);
	xyzwpr << T[0], T[1], T[2], RPW_euler[2], RPW_euler[1], RPW_euler[0];
	return xyzwpr;
}

Eigen::Matrix4f HandEyeCalib::CalBoardToCameraMatrix(std::vector<cv::Point2f> imgPoints, Eigen::Matrix3f camintr, Eigen::VectorXf camdist)
{
	// construct object virtual coordination system
	std::vector<cv::Point3f> objectPoints;

	for (int y = 0; y < board_.rows; ++y)
	{
		for (int x = 0; x < board_.cols; ++x)
		{
			objectPoints.push_back(cv::Point3f(x * board_.space, y * board_.space, 0));
		}
	}

	Mat rvecs, tvecs, R(3, 3, CV_64F);
	Matrix4f board2cam;
	Mat rod(3, 3, CV_64F);
	solvePnP(objectPoints, imgPoints, toCvMat(camintr), toCvMat(camdist), rvecs, tvecs);
	Rodrigues(rvecs, rod);
	board2cam = RT2HomogeneousMatrix(toEigenMatrix(rod), toEigenVector(tvecs));

	return board2cam;
}

// Eigen::Matrix4f HandEyeCalib::CalBoardToCameraMatrix(const vector<cv::Point2f>& PointBuf,
// 	const Mat& pcdImg, const Matrix3f& camintr, const CalibBoard caliboard)
// {
// 	cv::Size boardSize;
// 	boardSize.width = caliboard.cols;
// 	boardSize.height = caliboard.rows;

// 	std::vector<std::vector<cv::Point2f>> ImagePoints;
// 	std::vector<cv::Point3f> point_incam;
// 	Matrix4f m_calib2cam;
// 	Matrix4f m_cam2calib;
// 	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
// 	Point2f point_org = PointBuf[0];
// 	Point2f point_xdir = PointBuf[boardSize.width - 1];
// 	Point2f point_ydir = PointBuf[boardSize.width * (boardSize.height - 1)];
// 	Point2f point_end = PointBuf[(boardSize.width) * (boardSize.height) - 1];
// 	Mat grid_rec = cv::Mat::zeros(pcdImg.rows, pcdImg.cols, CV_8UC1);
// 	vector<cv::Point> contour;
// 	contour.reserve(4);
// 	contour.push_back(point_org);
// 	contour.push_back(point_xdir);
// 	contour.push_back(point_end);
// 	contour.push_back(point_ydir);
// 	vector<std::vector<cv::Point>> contours;
// 	contours.push_back(contour);
// 	cv::polylines(grid_rec, contours, true, cv::Scalar(255), 2, cv::LINE_AA);
// 	cv::fillPoly(grid_rec, contours, cv::Scalar(255));
// 	Point3f point;
// 	for (int i = 0; i < grid_rec.rows; i++) {
// 		for (int j = 0; j < grid_rec.cols; j++) {
// 			if (grid_rec.at<uchar>(i, j) != 0) {
// 				point.x = pcdImg.at<cv::Vec3f>(i, j)[0];
// 				point.y = pcdImg.at<cv::Vec3f>(i, j)[1];
// 				point.z = pcdImg.at<cv::Vec3f>(i, j)[2];
// 				point_incam.push_back(point);
// 			}
// 		}
// 	}
// 	// Calculate the focal point of the camera gaze line and plane
// 	// [ A  B  C    ][x]   [-D ]
// 	// | fx 0  cx-u ||y| = | 0 |
// 	// [ 0  fy cy-v ][z]   [ 0 ]
// 	float A, B, C, D, fx, fy, cx, cy;
// 	SinglePlaneFit(point_incam, A, B, C, D);

// 	// Calculate the extrinsic transformation matrix
// 	cv::Point3f Porg, Px, Py;
// 	Vector3f xdir, ydir, pln_x, pln_y, pln_z, plane_tvec;
// 	D *= 1000;
// 	Porg = CalcXYZByGazeline(camintr, A, B, C, D, point_org);
// 	Px = CalcXYZByGazeline(camintr, A, B, C, D, point_xdir);
// 	Py = CalcXYZByGazeline(camintr, A, B, C, D, point_ydir);
// 	xdir[0] = Px.x - Porg.x;
// 	xdir[1] = Px.y - Porg.y;
// 	xdir[2] = Px.z - Porg.z;
// 	ydir[0] = Py.x - Porg.x;
// 	ydir[1] = Py.y - Porg.y;
// 	ydir[2] = Py.z - Porg.z;
// 	pln_x = xdir / sqrt(xdir.dot(xdir));
// 	pln_y = ydir / sqrt(ydir.dot(ydir));
// 	pln_z = pln_x.cross(pln_y);
// 	pln_y = pln_z.cross(pln_x);
// 	pln_x = pln_y.cross(pln_z);
// 	Matrix3f plane_rmat;
// 	plane_rmat.col(0) = pln_x;
// 	plane_rmat.col(1) = pln_y;
// 	plane_rmat.col(2) = pln_z;
// 	plane_tvec[0] = Porg.x;
// 	plane_tvec[1] = Porg.y;
// 	plane_tvec[2] = Porg.z;

// 	Matrix3f plane_rmat_ortho = plane_rmat.householderQr().householderQ();

// 	m_calib2cam.setIdentity();
// 	m_calib2cam.block<3, 3>(0, 0) << plane_rmat_ortho;
// 	m_calib2cam.block<3, 1>(0, 3) << plane_tvec;

// 	return m_calib2cam;
// }

cv::Point3f HandEyeCalib::CalcXYZByGazeline(const Eigen::Matrix3f& camIntr, float& PA, float& PB,
	float& PC, float& PD, const cv::Point2f& uv) {
	//--[ A  B  C    ][x]   [-D ]
	//--| fx 0  cx-u ||y| = | 0 |
	//--[ 0  fy cy-v ][z]   [ 0 ]
	Point3f XYZ;
	Matrix3f mat1 = Matrix3f::Identity();
	Matrix3f mat1_inv = Matrix3f::Identity();
	Vector3f vec1, vecXYZ;
	double fx = camIntr(0, 0);
	double fy = camIntr(1, 1);
	double cx = camIntr(0, 2);
	double cy = camIntr(1, 2);
	mat1(0, 0) = PA; mat1(0, 1) = PB; mat1(0, 2) = PC;
	mat1(1, 0) = fx; mat1(1, 1) = 0;  mat1(1, 2) = cx - (int)(uv.x + 0.5);
	mat1(2, 0) = 0;  mat1(2, 1) = fy; mat1(2, 2) = cy - (int)(uv.y + 0.5);
	vec1[0] = -PD; vec1[1] = 0; vec1[2] = 0;
	mat1_inv = mat1.inverse();
	vecXYZ = mat1_inv * vec1;
	XYZ.x = vecXYZ[0];
	XYZ.y = vecXYZ[1];
	XYZ.z = vecXYZ[2];
	return XYZ;
}

cv::Mat HandEyeCalib::toCvMat(const Eigen::MatrixXf& mtr_eigen)
{
	Mat mtr_cv;
	eigen2cv(mtr_eigen, mtr_cv);
	return mtr_cv;
}

Eigen::MatrixXf HandEyeCalib::toEigenMatrix(const Mat& mtr_cv)
{
	MatrixXf mtr_eigen;
	cv2eigen(mtr_cv, mtr_eigen);
	return mtr_eigen;
}

Eigen::VectorXf HandEyeCalib::toEigenVector(const Mat& vec_cv)
{
	
	VectorXf vec_eigen;
	cv2eigen(vec_cv, vec_eigen);		// Only column vectors are allowed
	return vec_eigen;
}


bool HandEyeCalib::isRotatedMatrix(const Eigen::Matrix3f& R)
{
	if ((R * R.transpose()).isIdentity() &&
		abs(R.determinant() - 1) < 1e-6)
		return true;
	else
		return false;
}

bool HandEyeCalib::isInVector(const int idx, std::vector<int> idxVector)
{
	bool sta = false;

	if (idxVector.empty())
		return sta;

	for (size_t i = 0; i < idxVector.size(); i++)
	{
		if (idxVector[i] == idx) {
			sta = true;
			break;
		}
		else {
			sta = false;
			continue;
		}
	}
	return sta;
}

