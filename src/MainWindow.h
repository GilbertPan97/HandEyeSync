#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Prevent windows.h from including winsock.h
    #include <windows.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
#else
    #include <ifaddrs.h>
    #include <net/if.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif
#endif

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"
#include "DockWidgetViewer.h"
#include "DockWidgetLogger.h"
#include "DockWidgetBrowser.h"
#include "DockWidgetProperty.h"
#include "ProfileParser.h"
#include "FanucParser.h"
#include "PsCalibrator.h"
#include "PsAlgorithm.h"
#include "DataProcessor.h"
#include "LineScannerInterface.h"
#include "ISightServer.h"
#include "ThreadWorker.h"

#include <QMainWindow>
#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QToolButton>
#include <QList>
#include <QHBoxLayout>
#include <QThread>
#include <QPointer>
#include <nlohmann/json.hpp>

// Alias for a point that includes x, y, z, w, p, r
using FanucRobPose = std::vector<double>;

enum class SensorType {
    ProfileScanner,
    ImageCamera
};

// Define a struct to hold adapter information
struct AdapterInfo {
    std::string name;           // Adapter name
    std::string description;    // Adapter description
    std::string ipAddress;      // IP address
    std::string subnetMask;     // Subnet mask
    std::string defaultGateway;  // Default gateway

    // Constructor to initialize adapter information
    AdapterInfo(const std::string &name, const std::string &description,
                const std::string &ipAddress, const std::string &subnetMask,
                const std::string &defaultGateway)
        : name(name), description(description), ipAddress(ipAddress),
          subnetMask(subnetMask), defaultGateway(defaultGateway) {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();
    void createToolBar();
    void createProgressWidget();
    void setToolBarGroup(QList<QToolButton*> buttonList, QString groupTitle);
    void setToolBarGroup(QHBoxLayout* ctlWidgetLayout, QString groupTitle);
    void setWidgetProgress(int prog);
    
    void loadSettings();
    void saveSettings();

    std::vector<AdapterInfo> getNetworkAdapters();

    QString sensorTypeToString(SensorType sensorType);

    std::vector<std::vector<cv::Point3f>> convertPointsSetBuffer(const std::vector<RenderData>& pointsSetBuffer);
    std::vector<Eigen::Vector<float, 6>> convertRobDataBuffer(const std::vector<FanucRobPose>& robDataBuffer);
    ProfileSheet parseProfilePointToProfileSheet(RenderData data, int index, std::string feature_type, std::string path);
    std::vector<ProfileSheet> parseProfilePointsToProfileSheets(const std::vector<RenderData>& pointsSetBuffer, 
                                                                std::vector<cv::Point3f> features,
                                                                std::string feature_type,
                                                                std::vector<std::string> paths);
    std::vector<cv::Point3f> extractFeaturePointsFromProfileSheet(const std::vector<ProfileSheet>& profileSheets);
    void writeFeaturePointsToProfileSheets(const std::vector<cv::Point3f>& points, std::string type, 
                                           std::vector<ProfileSheet>& profileSheets);
    std::pair<double, double> projectToXozPlane(const cv::Point3f& point);
    void replaceProfileSheet(std::vector<ProfileSheet>& profiles, const ProfileSheet& newProfile);
    void saveProfileToFile(const RenderData& profile, const ProfileSheet& sheet);
    std::vector<std::pair<double, double>> convertToRenderData(const ProfileData& data);
    void initialDataIndex(size_t number_dataset);
    std::pair<int, int> getDataIndexAlloc(std::vector<int> dataIndex);
    void showCalibrationResult(const Eigen::Matrix4f& matrix, const Eigen::Vector<float, 6>& vec, float calibError);
    QString matrixToQString(const Eigen::Matrix4f& matrix);
    QString vectorToQString(const Eigen::Vector<float, 6>& vec);

signals:
    void sensorConnStatue(bool checked);

private slots:
    // Placeholder slots for menu actions
    void newFile();
    void openFile();
    void saveFile();
    void onAddImgActionTriggered();
    void onAddRobActionTriggered();
    void onSettingButtonReleased();
    void onRunButtonReleased();
    void showScanCameraDialog(QAction *actBtn);
    void onIpConnTriggered();
    void updateSeneorInfoGroupBox(QGroupBox *statusGroupBox, const CameraInfo &curCamInfo_);
    void updateCalibModelGroupBox(QGroupBox *modelGroupBox, nlohmann::json &calibMap);

    void onPlayToggled(bool ckecked);
    void onCaptureClicked();
    void replotSensorData();

private:
    QPointer<QToolBar> topToolBar_;              // Top toolbar
    QPointer<QDialog> progressWidget_;
    QPointer<QProgressBar> progressBar_;

    CameraInfo curCamInfo_;             // Store current selected sensor information
    LineScannerInterface sensorApi_;    // Make sensorApi a member variable
    QPointer<ThreadWorker> grabWorker_;
    QPointer<QThread> grabThread_;
    bool isGrabing_;

    ISightServer robConnServer_;

    // DockWidgets
    QPointer<ads::CDockManager> dockManager_;
    QList<QPointer<ads::CDockWidget>> dockWidgets_;
    QPointer<DockWidgetViewer> viewerWin_;
    QPointer<DockWidgetLogger> logWin_;
    QPointer<DockWidgetBrowser> browserWin_;
    QPointer<DockWidgetProperty> propertyWin_;

    // Calibration dataset
    std::vector<RenderData> profilesBuffer_;
    std::vector<FanucRobPose> robDataBuffer_;
    std::vector<ProfileSheet> profileSheets_;
    std::vector<int> dataIndex_;

    // Calibration configuration
    SensorType sensorType_;
    nlohmann::json calibMap_;
};

#endif // MAINWINDOW_H
