//
// Created by sun on 2020/2/27.
//

#include <iostream>
#include <fstream>
#include "stream_motion_controller.h"
//#include "raw_controller.h"


int main(){
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [thread %t] %v");
#if 0
    const char *IP = "192.168.1.1";
    FANUC::StreamMotionController controller(IP);
    controller.start();

    std::list<Eigen::VectorXd> path;
    path.push_back((Eigen::VectorXd(6) << -89.0, -11, -16, -1.0, -67.0, 0.0).finished());
    path.push_back((Eigen::VectorXd(6) << -68, -11, -16, -1, -28, 0.0).finished());
    path.push_back((Eigen::VectorXd(6) << -68, 1, -16, -1, -28, 0.0).finished());
    path.push_back((Eigen::VectorXd(6) << -43, 1, 11, -1, -28, 0.0).finished());
    path.push_back((Eigen::VectorXd(6) << -43, 1, -9, -1, -28, 0.0).finished());

    controller.moveJoint(path,0.5);
    controller.setIO(FANUC::StreamMotionController::IOType::F,2, true);
    controller.moveHome();
    controller.stop();
    controller.loop();
#endif

#if 0
    const char *IP = "192.168.1.171";
    FANUC::StreamMotionController controller(IP);
    controller.start();
    std::list<Eigen::VectorXd> path;
    std::ifstream input_file("/home/sun/PycharmProjects/fanuc_utils/cadpts.txt");
    if (!input_file.is_open()) {
        std::cout << "failed to open !\n";
    } else {
        // Read the Data from the file
        // as String Vector
        std::vector<std::string> row;
        std::string line, word, temp;
        int i = 0;
        while (input_file >> line) {
            row.clear();
            std::stringstream s(line);
            // read every column data of a row and
            // store it in a string variable, 'word'
            while (getline(s, word, ',')) {
                // add all the column data
                // of a row to a vector
                row.push_back(word);
            }
            i++;
//            if (i > 100){
//                break;
//            }
            // convert string to integer for comparision
            path.push_back((Eigen::VectorXd(6)<< stod(row[0])+500, stod(row[1]), stod(row[2])+20, -stod(row[3]),stod(row[4]), 0.0).finished());
        }
    }
//    path.push_back((Eigen::VectorXd(6) << 100.0, 60.0, 40.0, 0.0, 0.0, 0.0).finished());
//    path.push_back((Eigen::VectorXd(6) << 0.0, 0.0, 0.0, 0.0, -90, 0.0).finished());
//    path.push_back((Eigen::VectorXd(6) << 470.0, 0.0, 395.0, -180.0, 0.0, 0.0).finished());
    controller.moveCarteSian(path,0.1);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    controller.moveHome(0.5);
    controller.stop();
    controller.loop();
#endif


#if 1
    const char *IP = "169.254.249.94";
    FANUC::StreamMotionController controller(IP);
    controller.start();
    std::list<Eigen::VectorXd> path;
    std::ifstream input_file("/home/fanucvision/Downloads/libfanucsm/data/data2.csv");
    if (!input_file.is_open()) {
        std::cout << "failed to open !\n";
    } else {
        // Read the Data from the file
        // as String Vector
        std::vector<std::string> row;
        std::string line, word, temp;
        int i = 0;
        while (input_file >> line) {
            row.clear();
            std::stringstream s(line);
            // read every column data of a row and
            // store it in a string variable, 'word'
            while (getline(s, word, ',')) {
                // add all the column data
                // of a row to a vector
                row.push_back(word);
            }
            i++;
            // convert string to integer for comparision
            path.push_back((Eigen::VectorXd(6)<< stod(row[0]), stod(row[1]), stod(row[2])+300, (stod(row[3])<0?stod(row[3])+360:stod(row[3])),stod(row[4]), stod(row[5])).finished());
        }
    }

    Eigen::Affine3d utool = Eigen::Affine3d::Identity();
    utool.translation() << 0,0,-250;


    auto xyzwpr2affine = [](Eigen::VectorXd& xyzwpr)->Eigen::Affine3d{
        Eigen::Matrix3d R;
        R = Eigen::AngleAxisd(xyzwpr[5]*M_PI/180.0, Eigen::Vector3d::UnitZ()) // static xyz
            * Eigen::AngleAxisd(xyzwpr[4]*M_PI/180.0, Eigen::Vector3d::UnitY())
            * Eigen::AngleAxisd(xyzwpr[3]*M_PI/180.0, Eigen::Vector3d::UnitX());
        Eigen::Affine3d ret = Eigen::Affine3d::Identity();
        ret.prerotate(R);
        (ret.translation() << xyzwpr[0], xyzwpr[1], xyzwpr[2]).finished();
        return ret;
    };

    auto affine2xyzwpr = [](Eigen::Affine3d& affine, Eigen::VectorXd& pt)->Eigen::VectorXd{
        const Eigen::Matrix3d& R = affine.rotation();
        Eigen::Vector3d wpr = R.eulerAngles(2,1,0);
        Eigen::Vector3d trans = affine.translation();
        return (Eigen::VectorXd(6)<< trans[0], trans[1], trans[2], -180,0,0).finished();
    };
    for (auto& pt:  path) {
//        auto t_pt = xyzwpr2affine(pt);
//        auto res_pt = t_pt*utool;
//        pt = affine2xyzwpr(res_pt, pt);
        std::cout << std::endl;
        std::cout << pt << std::endl;
    }

    controller.moveCarteSian(path,0.3);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    controller.moveHome(0.5);
    controller.stop();
    controller.loop();
#endif

#if 0
    std::list<Eigen::VectorXd> path;
    std::ifstream input_file("/home/sun/CLionProjects/libfanucsm/data/data2.csv");
    if (!input_file.is_open()) {
        std::cout << "failed to open !\n";
    } else {
        // Read the Data from the file
        // as String Vector
        std::vector<std::string> row;
        std::string line, word, temp;
        int i = 0;
        while (input_file >> line) {
            row.clear();
            std::stringstream s(line);
            // read every column data of a row and
            // store it in a string variable, 'word'
            while (getline(s, word, ',')) {
                // add all the column data
                // of a row to a vector
                row.push_back(word);
            }
            i++;
            // convert string to integer for comparision
            path.push_back((Eigen::VectorXd(6)<< stod(row[0]), stod(row[1]), stod(row[2])+200, stod(row[3]),stod(row[4]), stod(row[5])).finished());
        }
    }
    auto xyzwpr2affine = [](Eigen::VectorXd& xyzwpr)->Eigen::Affine3d{
        Eigen::Matrix3d R;
        R = Eigen::AngleAxisd(xyzwpr[5]*M_PI/180.0, Eigen::Vector3d::UnitZ()) // static xyz
            * Eigen::AngleAxisd(xyzwpr[4]*M_PI/180.0, Eigen::Vector3d::UnitY())
            * Eigen::AngleAxisd(xyzwpr[3]*M_PI/180.0, Eigen::Vector3d::UnitX());
        Eigen::Affine3d ret = Eigen::Affine3d::Identity();
        ret.prerotate(R);
        (ret.translation() << xyzwpr[0], xyzwpr[1], xyzwpr[2]).finished();
        return ret;
    };

    auto affine2xyzwpr = [](Eigen::Affine3d& affine, Eigen::VectorXd& pt)->Eigen::VectorXd{
        const Eigen::Matrix3d& R = affine.rotation();
        Eigen::Vector3d wpr = R.eulerAngles(2,1,0);
        Eigen::Vector3d trans = affine.translation();
        return (Eigen::VectorXd(6)<< trans[0], trans[1], trans[2], -180,0,0).finished();
    };
//    Eigen::Affine3d utool = Eigen::Affine3d::Identity();
//    utool.translation() << 0,0,-250;
//    for (auto& pt:  path) {
//        auto t_pt = xyzwpr2affine(pt);
//        auto res_pt = t_pt*utool;
//        pt = affine2xyzwpr(res_pt, pt);
//        std::cout << std::endl;
//        std::cout << pt << std::endl;
//    }
    auto  trajectory = trajectory_factory(path,1,InterpolationType::QBCART);
    auto callback = std::bind(&Trajectory::operator(), trajectory, std::placeholders::_1);
    double_t time = 0.0;
    bool finished = false;
    do{
        auto data = callback(time);
        finished = data.finished;
        time += 0.008;
        std::cout << data.world.x << "," <<data.world.y << ","<<data.world.z << ","<<data.world.w << ","<<data.world.p << ","<<data.world.r << std::endl;
    }while (not finished);
#endif
}
