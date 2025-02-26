#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <nlohmann/json.hpp>

// Alias for a point that includes x, y, z, w, p, r
using FanucRobPose = std::vector<double>;
using ProfilePoints = std::vector<std::pair<double, double>>;

enum class SensorType {
    ProfileScanner,
    ImageCamera
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

    QString sensorTypeToString(SensorType sensorType);

    std::vector<std::vector<cv::Point3f>> convertPointsSetBuffer(const std::vector<ProfilePoints>& pointsSetBuffer);
    std::vector<Eigen::Vector<float, 6>> convertRobDataBuffer(const std::vector<FanucRobPose>& robDataBuffer);
    std::vector<ProfileSheet> parseProfilePointsToProfileSheets(const std::vector<ProfilePoints>& pointsSetBuffer, 
                                                                std::vector<cv::Point3f> features, 
                                                                std::vector<std::string> paths);
    std::vector<cv::Point3f> extractFeaturePointsFromProfileSheet(const std::vector<ProfileSheet>& profileSheets);
    void writeFeaturePointsToProfileSheets(const std::vector<cv::Point3f>& points, std::vector<ProfileSheet>& profileSheets);
    std::pair<double, double> projectToXozPlane(const cv::Point3f& point);
    void replaceProfileSheet(std::vector<ProfileSheet>& profiles, const ProfileSheet& newProfile);
    void saveProfileToFile(const ProfilePoints& profile, const ProfileSheet& sheet);

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

private:
    QToolBar *topToolBar_;              // Top toolbar
    QDialog *progressWidget_;
    QProgressBar *progressBar_;

    CameraInfo curCamInfo_;             // Store current selected sensor information
    LineScannerInterface sensorApi_;    // Make sensorApi a member variable

    // DockWidgets
    ads::CDockManager* dockManager_;
    QList<ads::CDockWidget*> dockWidgets_;
    DockWidgetViewer* viewerWin_;
    DockWidgetLogger* logWin_;
    DockWidgetBrowser* browserWin_;
    DockWidgetProperty* propertyWin_;

    // Calibration dataset
    std::vector<ProfilePoints> profilesBuffer_;
    std::vector<FanucRobPose> robDataBuffer_;
    std::vector<ProfileSheet> profileSheets_;

    // Calibration configuration
    SensorType sensorType_;
    nlohmann::json calibMap_;
};

#endif // MAINWINDOW_H
