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

struct ProfileSheet {
    int profileIndex;              // Profile Index
    int pointCount;                // Number of points in profile
    cv::Point3f featurePoint;      // Feature point in camera frame (for example, center of sphere or edge corner)
    bool enableFilter;             // Whether filter is enabled for this profile
    std::string filterType;        // Filter type (e.g., "Gaussian", "Median", etc.)

    ProfileSheet(int index = -1, int count = 0, const cv::Point3f& feature = cv::Point3f(0, 0, 0), bool enable = false, const std::string& filter = "")
        : profileIndex(index), pointCount(count), featurePoint(feature), enableFilter(enable), filterType(filter) {}
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
    std::vector<cv::Point3f> extractFeaturePointsFromProfileSheet(const std::vector<ProfileSheet>& profileSheets);
    void writeFeaturePointsToProfileSheets(const std::vector<cv::Point3f>& points, std::vector<ProfileSheet>& profileSheets);

private slots:
    // Placeholder slots for menu actions
    void newFile();
    void openFile();
    void saveFile();
    void onAddImg1ActionTriggered();
    void onAddImg2ActionTriggered();
    void onAddRob1ActionTriggered();
    void onAddRob2ActionTriggered();
    void onSettingButtonReleased();
    void onRunButtonReleased();

private:
    QToolBar *topToolBar_;         // Top toolbar
    QDialog *progressWidget_;
    QProgressBar *progressBar_;

    // DockWidgets
    ads::CDockManager* dockManager_;
    QList<ads::CDockWidget*> dockWidgets_;
    DockWidgetViewer* viewerWin_;
    DockWidgetLogger* logWin_;
    DockWidgetBrowser* browserWin_;
    DockWidgetProperty* propertyWin_;

    // Calibration dataset
    std::vector<ProfilePoints> pointsSetBuffer_;
    std::vector<FanucRobPose> robDataBuffer_;
    std::vector<ProfileSheet> featuresSheet_;
    std::vector<cv::Point3f> feature_pnts;          // Feature points in camera frame (sphere center or edge corner)

    // Calibration configuration
    SensorType sensorType_;
    nlohmann::json calibMap_;
};

#endif // MAINWINDOW_H
