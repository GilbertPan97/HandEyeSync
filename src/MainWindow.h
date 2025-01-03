#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"
#include "DockWidgetViewer.h"
#include "DockWidgetLogger.h"
#include "DockWidgetBrowser.h"
#include "DockWidgetProperty.h"

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();
    void createToolBar();
    void setToolBarGroup(QList<QToolButton*> buttonList, QString groupTitle);
    void setToolBarGroup(QHBoxLayout* ctlWidgetLayout, QString groupTitle);
    
    void loadSettings();
    void saveSettings();

    double extractNumericValueFromFilename(const QString& filename);
    std::vector<std::vector<std::pair<double, double>>> parseProfileFiles(const QString& folderPath, const QString& type);

private slots:
    // Placeholder slots for menu actions
    void newFile();
    void openFile();
    void saveFile();
    void onAddImg1ActionTriggered();
    void onAddImg2ActionTriggered();
    void onAddRob1ActionTriggered();
    void onAddRob2ActionTriggered();

private:
    QToolBar *topToolBar_;         // Top toolbar

    ads::CDockManager* dockManager_;
    QList<ads::CDockWidget*> dockWidgets_;
    DockWidgetViewer* viewerWin_;
    DockWidgetLogger* logWin_;
    DockWidgetBrowser* browserWin_;
    DockWidgetProperty* propertyWin_;
    std::vector<std::vector<std::pair<double, double>>> pointsSetBuffer_;
};

#endif // MAINWINDOW_H
