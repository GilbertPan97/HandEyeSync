#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QTextEdit>
#include <QWidget>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QMenuBar>
#include <QActionGroup>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Load the external QSS file for dark theme (use style.qss)
    QFile file(":styles/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString style = stream.readAll();
        setStyleSheet(style);
    }

    // Create the central widget (text editor)
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    dockManager_ = new ads::CDockManager(this);
    ads::CDockWidget* viewerWin = new ads::CDockWidget("Viewer", this);
    ads::CDockWidget* logWin = new ads::CDockWidget("Logger", this);
    ads::CDockWidget* browserWin = new ads::CDockWidget("Data Brower", this);

    dockManager_->addDockWidget(ads::TopDockWidgetArea, logWin);
    auto topDockWidgetArea = dockManager_->addDockWidget(ads::TopDockWidgetArea, viewerWin);
    auto leftDockWidgetArea = dockManager_->addDockWidget(ads::LeftDockWidgetArea, browserWin);

    int width = this->width();
    int height = this->height();
    dockManager_->setSplitterSizes(topDockWidgetArea, {static_cast<int>(height * 0.7), static_cast<int>(height * 0.3)});
    dockManager_->setSplitterSizes(leftDockWidgetArea, {static_cast<int>(width * 0.15), static_cast<int>(width * 0.85)});
    centralLayout->addWidget(dockManager_);

    dockWidgets_ << viewerWin << logWin << browserWin;

    // Adjust layout margins to remove any gaps
    centralLayout->setContentsMargins(0, 0, 0, 0);  // No margins between widgets
    centralLayout->addWidget(dockManager_);

    setCentralWidget(centralWidget);  // Set it as the central widget

    // Create menu and toolbar by calling the functions
    createMenuBar();
    createToolBar();

    // Set the window title
    setWindowTitle("Qt Toolbar Layout Example");

    // Adjust the DockOptions to avoid any unwanted gap
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AnimatedDocks);

    // Load saved settings (layout, window size, position)
    loadSettings();
}

MainWindow::~MainWindow()
{
    // Save window settings before closing
    saveSettings();
}

// Function to create the menu bar
void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *newMenuAction = fileMenu->addAction("New Section");
    QAction *openMenuAction = fileMenu->addAction("Open Section");
    QAction *saveMenuAction = fileMenu->addAction("Save Section");
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("Exit");

    QMenu *deviceMenu = menuBar()->addMenu("Device");
    QAction *connectAction = deviceMenu->addAction("Connect");
    QAction *disconnectAction = deviceMenu->addAction("Disconnect");

    QMenu *editMenu = menuBar()->addMenu("Edit");
    QAction *copyAction = editMenu->addAction("Copy");
    QAction *pasteAction = editMenu->addAction("Paste");
    QAction *cutAction = editMenu->addAction("Cut");

    QMenu *windowMenu = menuBar()->addMenu("Window");
    QAction *maximizeAction = windowMenu->addAction("Maximize");
    QAction *minimizeAction = windowMenu->addAction("Minimize");

    QMenu *helpMenu = menuBar()->addMenu("Help");
    QAction *aboutAction = helpMenu->addAction("About");

    // Connect actions to slots (placeholders for now)
    connect(newMenuAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openMenuAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveMenuAction, &QAction::triggered, this, &MainWindow::saveFile);
    // connect(exitAction, &QAction::triggered, this, &QApplication::quit);
}

// Function to create the toolbar
void MainWindow::createToolBar()
{
    // Create the top toolbar (horizontal)
    topToolBar = addToolBar("Top Toolbar");  // Add a horizontal toolbar

    // Ensure text is shown under the icon
    topToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // Create actions and set icons
    QAction *newAction = topToolBar->addAction("New");
    newAction->setIcon(QIcon(":/icons/folder.png"));
    newAction->setText("New\nSection");
    newAction->setToolTip("Create a new section");

    QAction *openAction = topToolBar->addAction("Open");
    openAction->setIcon(QIcon(":/icons/open.png"));
    openAction->setText("Open\nSection");

    QAction *saveAction = topToolBar->addAction("Save");
    saveAction->setIcon(QIcon(":/icons/save-file.png"));
    saveAction->setText("Save\nSection");

    // Add actions to the toolbar
    topToolBar->addAction(newAction);
    topToolBar->addAction(openAction);
    topToolBar->addAction(saveAction);
    
    topToolBar->addSeparator();

    // Add calibration data
    QMenu *addImgMenu = new QMenu(this);
    QAction *addImage1Action = addImgMenu->addAction("Add Image 1");
    QAction *addImage2Action = addImgMenu->addAction("Add Image 2");

    QAction *addImgAction = topToolBar->addAction("AddImgs");
    addImgAction->setIcon(QIcon(":/icons/add-images.png"));
    addImgAction->setText("Add\nImages");
    addImgAction->setMenu(addImgMenu);

    QAction *addRobAction = topToolBar->addAction("AddRobData");
    addRobAction->setIcon(QIcon(":/icons/add-robot.png"));
    addRobAction->setText("Add\nRobot Data");

    topToolBar->addSeparator();
}

// Placeholder slots for menu actions
void MainWindow::newFile()
{
    // Handle new file action
}

void MainWindow::openFile()
{
    // Handle open file action
}

void MainWindow::saveFile()
{
    // Handle save file action
}

void MainWindow::saveSettings()
{
    QSettings settings("Shanghai Fanuc", "HandEyeSync");

    // Save window size, position, and state (maximized or minimized)
    settings.setValue("window/size", size());
    settings.setValue("window/position", pos());
    settings.setValue("window/state", saveState());  // Save the window state (maximized, minimized)

    // Print saved window size, position, and state for debugging
    qDebug() << "Saving window size:" << size();
    qDebug() << "Saving window position:" << pos();
    qDebug() << "Saving window state:" << (isMaximized() ? "Maximized" : (isMinimized() ? "Minimized" : "Normal"));

    // Save dock widget states
    settings.setValue("dockState", dockManager_->saveState());
}

void MainWindow::loadSettings()
{
    QSettings settings("Shanghai Fanuc", "HandEyeSync");

    // Restore window size, position, and state (maximized or minimized)
    QSize size = settings.value("window/size", QSize(800, 600)).toSize();
    QPoint pos = settings.value("window/position", QPoint(100, 100)).toPoint();
    resize(size);

    // Restore window state (maximized, minimized, normal)
    Qt::WindowState state = static_cast<Qt::WindowState>(settings.value("window/state", Qt::WindowNoState).toInt());
    setWindowState(state);

    // Print restored window size, position, and state for debugging
    qDebug() << "Restoring window size:" << size;
    qDebug() << "Restoring window position:" << pos;
    qDebug() << "Restoring window state:" << (state == Qt::WindowMaximized ? "Maximized" : 
                                              (state == Qt::WindowMinimized ? "Minimized" : "Normal"));

    // // Restore dock widget states
    // QByteArray dockState = settings.value("dockState").toByteArray();
    // if (!dockState.isEmpty()) {
    //     dockManager_->restoreState(dockState);
    // }
}

