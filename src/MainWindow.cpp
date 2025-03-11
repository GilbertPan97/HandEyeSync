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
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <opencv2/opencv.hpp>

#include <regex>

MainWindow::MainWindow(QWidget *parent)
    : grabWorker_(nullptr), grabThread_(nullptr),
      QMainWindow(parent)
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
    viewerWin_ = new DockWidgetViewer("Viewer", this);
    logWin_ = new DockWidgetLogger("Logger", this);
    browserWin_ = new DockWidgetBrowser("Data Browser", this);
    propertyWin_ = new DockWidgetProperty("Property Browser", this);

    viewerWin_->setFeatures(viewerWin_->features() & ~QDockWidget::DockWidgetClosable);
    logWin_->setFeatures(logWin_->features() & ~QDockWidget::DockWidgetClosable);
    browserWin_->setFeatures(browserWin_->features() & ~QDockWidget::DockWidgetClosable);
    propertyWin_->setFeatures(propertyWin_->features() & ~QDockWidget::DockWidgetClosable);

    dockManager_->addDockWidget(ads::TopDockWidgetArea, viewerWin_);
    auto rightDockWidgetArea = dockManager_->addDockWidget(ads::RightDockWidgetArea, propertyWin_);
    auto bottomDockWidgetArea = dockManager_->addDockWidget(ads::BottomDockWidgetArea, logWin_);
    auto leftDockWidgetArea = dockManager_->addDockWidget(ads::LeftDockWidgetArea, browserWin_);

    int width = this->width();
    int height = this->height();
    dockManager_->setSplitterSizes(bottomDockWidgetArea, {static_cast<int>(height * 0.75), static_cast<int>(height * 0.25)});
    dockManager_->setSplitterSizes(leftDockWidgetArea, {static_cast<int>(width * 0.15), static_cast<int>(width * 0.85)});
    dockManager_->setSplitterSizes(rightDockWidgetArea, {static_cast<int>(width * 0.85 * 0.7), static_cast<int>(width * 0.85 * 0.3)});
    centralLayout->addWidget(dockManager_);

    dockWidgets_ << viewerWin_ << logWin_ << browserWin_ << propertyWin_;

    // Connect propertyWin_ and viewerWin_, feature point in viewerWin_ will editable when pick button trigered
    connect(propertyWin_, &DockWidgetProperty::pickFeatureStatus, viewerWin_, &DockWidgetViewer::onFeaturePickEnable);
    connect(viewerWin_, &DockWidgetViewer::updatedFeaturePoint, this, [this](ProfileSheet sheet) {
        // Check if the ProfileSheet is valid by verifying the profileIndex
        if (sheet.profileIndex < 0) {
            // Log an error message and return to prevent further processing
            logWin_->log("Error: Invalid profileIndex in received point!");
            return;
        }
    
        // Write the ProfileSheet to the properties window
        propertyWin_->writeProfileSheetToProperties(sheet, true);

        // Save profile and profileSheets_ to files
        saveProfileToFile(profilesBuffer_[sheet.profileIndex], sheet);
    
        // Attempt to replace the ProfileSheet in the profileSheets_
        try {
            replaceProfileSheet(profileSheets_, sheet);
        } catch (const std::exception& e) {
            // If an error occurs during replacement, catch the exception and log the error
            logWin_->log(QString("Error replacing ProfileSheet: ") + QString::fromStdString(e.what()));
        }
    });
    connect(this, &MainWindow::sensorConnStatue, viewerWin_, &DockWidgetViewer::onSensorOpsEnable);

    // Adjust layout margins to remove any gaps
    centralLayout->setContentsMargins(0, 0, 0, 0);  // No margins between widgets
    centralLayout->addWidget(dockManager_);

    setCentralWidget(centralWidget);  // Set it as the central widget

    // Create menu and toolbar by calling the functions
    createMenuBar();
    createToolBar();
    createProgressWidget();

    // Set the window title
    setWindowTitle("HandEyeSync");

    // Adjust the DockOptions to avoid any unwanted gap
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AnimatedDocks);

    // Load saved settings (layout, window size, position)
    loadSettings();
}

MainWindow::~MainWindow()
{
    // Save window settings before closing
    saveSettings();

    if (grabWorker_) {
        // Stop the tasks running in grabWorker_ and safely delete
        grabWorker_->stopGrabbing();
        grabWorker_->deleteLater();
        grabWorker_ = nullptr;
    }

    if (grabThread_) {
        // Stop the thread and wait for it to finish
        grabThread_->quit();    // Request the thread to exit
        grabThread_->wait();    // Wait for the thread to fully finish

        // Safely delete the thread object
        grabThread_->deleteLater();
        grabThread_ = nullptr;
    }

    // Delete all dock widgets
    for (auto dockWidget : dockWidgets_) {
        delete dockWidget;
    }
    delete dockManager_;
    dockWidgets_.clear();

    delete topToolBar_;
    delete progressWidget_;
    delete progressBar_;
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

    // Add Sszn, LMI, and FanucRobot menu items
    QAction *sensorAction = deviceMenu->addAction("Sensor");
    sensorAction->setCheckable(true);  

    connect(sensorAction, &QAction::triggered, this, [this, sensorAction]() { showScanCameraDialog(sensorAction); });
    deviceMenu->addSeparator();

    QAction *fanucRobotAction = deviceMenu->addAction("FanucRobot");
    fanucRobotAction->setCheckable(true);

    QMenu *editMenu = menuBar()->addMenu("Edit");
    QAction *copyAction = editMenu->addAction("Copy");
    QAction *pasteAction = editMenu->addAction("Paste");
    QAction *cutAction = editMenu->addAction("Cut");

    QMenu *windowMenu = menuBar()->addMenu("Window");
    // Dynamically add dock widgets to the window menu
    for (ads::CDockWidget* dockWidget : dockWidgets_) {
        QAction* dockAction = windowMenu->addAction(dockWidget->windowTitle());
        dockAction->setCheckable(true);

        // Initially set the checked state based on visibility
        dockAction->setChecked(!dockWidget->isClosed());

        // Connect the action to toggle dock widget visibility
        connect(dockAction, &QAction::triggered, this, [this, dockWidget](bool checked) {
            // If the dockWidget is not visible (i.e., it was closed), show it
            if (checked) {
                dockWidget->show(); // Reopen the dock widget
            } else {
                dockWidget->hide(); // Hide the dock widget
            }
        });

        // Connect to the signal when dockWidget's visibility changes
        connect(dockWidget, &ads::CDockWidget::visibilityChanged, this, [this, dockAction, dockWidget](bool visible) {
            // Update the menu item state when the dock widget visibility changes
            dockAction->setChecked(visible);
        });
    }

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
    topToolBar_ = addToolBar("Top Toolbar");  // Add a horizontal toolbar

    // Ensure text is shown under the icon
    topToolBar_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    /* ========================= Section Page ========================= */
    QToolButton *newButton = new QToolButton();
    newButton->setIcon(QIcon(":/icons/new-file.png"));
    newButton->setText("New Section");
    newButton->setToolTip("Create a new section");

    QToolButton *openButton = new QToolButton();
    openButton->setIcon(QIcon(":/icons/open.png"));
    openButton->setText("Open Section");

    QToolButton *saveButton = new QToolButton();
    saveButton->setIcon(QIcon(":/icons/save-file.png"));
    saveButton->setText("Save Section");

    // Group the buttons using setToolBarGroup
    QList<QToolButton*> sec_btnList = { newButton, openButton, saveButton };
    setToolBarGroup(sec_btnList, "CALIBRATION SECTION");
    
    topToolBar_->addSeparator();

    /* ========================= Dataset Page ========================= */
    QVBoxLayout *onlineCollectLayout = new QVBoxLayout(this);
    onlineCollectLayout->setSpacing(10);
    QPushButton *onlineCollectBtn = new QPushButton("Online Collect", this);
    onlineCollectBtn->setIcon(QIcon(":/icons/globe.png"));
    onlineCollectBtn->setCheckable(true);
    
    QHBoxLayout *collectLayout = new QHBoxLayout(this);
    QLabel *indexLabel = new QLabel("Index", this);
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItem("0");
    QPushButton *downloadButton = new QPushButton(this);
    downloadButton->setIcon(QIcon(":/icons/download.png"));
    downloadButton->setToolTip("Collect");
    indexLabel->setEnabled(false);
    comboBox->setEnabled(false);
    downloadButton->setEnabled(false);
    collectLayout->addWidget(indexLabel, 0, Qt::AlignVCenter);
    collectLayout->addWidget(comboBox, 0, Qt::AlignVCenter);
    collectLayout->addWidget(downloadButton, 0, Qt::AlignVCenter);

    // Widgets indexLabel, comboBox and downloadButton only editable when onlineCollectBtn toggled
    connect(onlineCollectBtn, &QPushButton::toggled, [=](bool checked) {
        if (checked) {
            // Check devices connect status
            if (!curCamInfo_.isConnected) {
                logWin_->log("Warning: No sensor connected.");
                return;
            }
            // If the button is pressed (checked), enable the controls and remove gray appearance
            indexLabel->setEnabled(true);
            comboBox->setEnabled(true);
            downloadButton->setEnabled(true);
            onlineCollectBtn->setText("Collecting... ");
            onlineCollectBtn->setStyleSheet("background-color: #4CAF50; color: white;");
        } 
        else {
            // If the button is released (unchecked), disable the controls and apply gray appearance
            indexLabel->setEnabled(false);
            comboBox->setEnabled(false);
            downloadButton->setEnabled(false);
            onlineCollectBtn->setText("Online Collect");  // Reset to the original text
            onlineCollectBtn->setStyleSheet("background-color: #444444; color: white;");
        }
    });

    onlineCollectLayout->addWidget(onlineCollectBtn);
    onlineCollectLayout->addLayout(collectLayout);

    // ~~~ Widget 1: Sensor type.
    QMenu *addSensorMenu = new QMenu(this);
    QAction *profileScannerAction = addSensorMenu->addAction("Profile Scanner");
    QAction *imageCameraAction = addSensorMenu->addAction("Image Camera");
    profileScannerAction->setCheckable(true);           // Set both actions as checkable
    imageCameraAction->setCheckable(true);
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(profileScannerAction);
    actionGroup->addAction(imageCameraAction);

    // Set Profile Scanner as the default selected action (checked)
    profileScannerAction->setChecked(true);
    sensorType_ = SensorType::ProfileScanner;

    // Connect the actions to slots to handle the toggling behavior
    connect(profileScannerAction, &QAction::toggled, this, [=](bool checked) {if (checked) sensorType_ = SensorType::ProfileScanner;});
    connect(imageCameraAction, &QAction::toggled, this, [=](bool checked) {if (checked) sensorType_ = SensorType::ImageCamera;});

    QToolButton *sensorSelButton = new QToolButton();
    sensorSelButton->setIcon(QIcon(":/icons/sensor.png"));
    sensorSelButton->setText("Select sensor");
    sensorSelButton->setMenu(addSensorMenu);
    sensorSelButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sensorSelButton->setPopupMode(QToolButton::MenuButtonPopup);

    // ~~~ Widget 2: Load Image Button.
    QToolButton *addImgButton = new QToolButton();
    addImgButton->setIcon(QIcon(":/icons/add-images.png"));
    addImgButton->setText("Load Images");
    addImgButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(addImgButton, &QToolButton::released, this, &MainWindow::onAddImgActionTriggered);

    // ~~~ Widget 3: Load Robot Data Button.
    QToolButton *addRobButton = new QToolButton();
    addRobButton->setIcon(QIcon(":/icons/add-robot.png"));
    addRobButton->setText("Load Robot Data");
    addRobButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(addRobButton, &QToolButton::released, this, &MainWindow::onAddRobActionTriggered);

    QHBoxLayout *datasetLayout = new QHBoxLayout(this);
    datasetLayout->addLayout(onlineCollectLayout);
    datasetLayout->addWidget(sensorSelButton);
    datasetLayout->addWidget(addImgButton);
    datasetLayout->addWidget(addRobButton);

    // Group the buttons using setToolBarGroup
    QList<QToolButton*> data_btnList = { sensorSelButton, addImgButton, addRobButton };
    setToolBarGroup(datasetLayout, "DATASET");
    topToolBar_->addSeparator();

    /* ========================= Calibrate Page ========================= */
    QVBoxLayout* calibTypeLayout = new QVBoxLayout(this);
    calibTypeLayout->setSpacing(10);
    QLabel *calibTypeLabel = new QLabel("Calibration Type:");
    calibTypeLayout->addWidget(calibTypeLabel);
    QRadioButton *eyeInHandRadioButton = new QRadioButton("Eye-In-Hand");
    QRadioButton *eyeToHandRadioButton = new QRadioButton("Eye-To-Hand");

    // Create a button group to ensure only one can be selected at a time
    QButtonGroup *calibTypeGroup = new QButtonGroup(this);
    calibTypeGroup->addButton(eyeInHandRadioButton);
    calibTypeGroup->addButton(eyeToHandRadioButton);

    // Add the radio buttons to the layout
    calibTypeLayout->addWidget(eyeInHandRadioButton);
    calibTypeLayout->addWidget(eyeToHandRadioButton);
    eyeInHandRadioButton->setChecked(true);
    calibMap_["CalibrType"] = "Eye-In-Hand";

    // Connect a single lambda function to handle both buttons
    connect(eyeInHandRadioButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked) {
            calibMap_["CalibrType"] = "Eye-In-Hand";
            logWin_->log("Calibration Type set to: Eye-In-Hand");
        }
    });
    connect(eyeToHandRadioButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked) {
            calibMap_["CalibrType"] = "Eye-To-Hand";
            logWin_->log("Calibration Type set to: Eye-To-Hand");
        }
    });

    // Calibrate setting and run button
    QMenu *calSettingMenu = new QMenu(this);
    QAction *calSet1Action = calSettingMenu->addAction("Method");
    QAction *calSet2Action = calSettingMenu->addAction("Algorithm");

    QToolButton *settingButton = new QToolButton(this);
    settingButton->setText("Configuration");
    settingButton->setIcon(QIcon(":/icons/setting.png"));
    settingButton->setToolTip("Calibration setting");
    settingButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(settingButton, &QPushButton::released, this, &MainWindow::onSettingButtonReleased);

    QToolButton *runButton = new QToolButton(this);
    runButton->setText("Run");
    runButton->setIcon(QIcon(":/icons/run.png"));
    runButton->setToolTip("Calibration exec");
    runButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(runButton, &QPushButton::released, this, &MainWindow::onRunButtonReleased);

    // Add the QToolButton to a button group (if you have a function like setGroupBtn for grouping buttons)
    QHBoxLayout* ctlWidgetLayout = new QHBoxLayout(this);
    ctlWidgetLayout->addLayout(calibTypeLayout);
    ctlWidgetLayout->addWidget(settingButton);
    ctlWidgetLayout->addWidget(runButton);
    setToolBarGroup(ctlWidgetLayout, "CALIBRATE");

    topToolBar_->addSeparator();

    /* ========================= Result Page ========================= */
    QToolButton *checkBtn = new QToolButton();
    checkBtn->setIcon(QIcon(":/icons/check.png"));
    checkBtn->setText("Check");
    checkBtn->setToolTip("Check calibration resuslt");

    QToolButton *exportBtn = new QToolButton();
    exportBtn->setIcon(QIcon(":/icons/export.png"));
    exportBtn->setText("Export");
    exportBtn->setToolTip("Export calibration resuslt");

    // Group the buttons to page
    QList<QToolButton*> result_btnList = { checkBtn, exportBtn };
    setToolBarGroup(result_btnList, "SOLVE RESULT");
    topToolBar_->addSeparator();

}

// Function to create the progress widget
void MainWindow::createProgressWidget()
{
    progressWidget_ = new QDialog(this);
    progressWidget_->setWindowTitle("Progress");
    progressWidget_->setFixedSize(300, 100);

    progressWidget_->setStyleSheet("background-color: #353535;");

    // Create layout and widgets
    QVBoxLayout *layout = new QVBoxLayout(progressWidget_);
    QLabel *label = new QLabel("Execution Progress:", progressWidget_);
    progressBar_ = new QProgressBar(progressWidget_);
    progressBar_->setRange(0, 100);
    progressBar_->setValue(0);

    // Add widgets to the layout
    layout->addWidget(label);
    layout->addWidget(progressBar_);

    progressWidget_->setLayout(layout);
    progressWidget_->hide();
}

void MainWindow::setToolBarGroup(QList<QToolButton*> buttonList, QString groupTitle)
{
    // Create a new QWidget container
    QWidget *widget = new QWidget();

    // Create a vertical layout to hold all the elements
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Set the spacing for the main vertical layout
    mainLayout->setSpacing(0);  // Remove spacing between widgets
    mainLayout->setContentsMargins(10, 5, 10, 5);  // Set left and right margins (10px), top and bottom 5px

    // Create a horizontal layout to place the QToolButtons
    QHBoxLayout *hLayout = new QHBoxLayout();

    // Set the spacing for the horizontal layout
    hLayout->setSpacing(3);  // Remove spacing between buttons
    hLayout->setContentsMargins(0, 5, 0, 5);  // No margins around the buttons

    // Iterate through the button list and add each QToolButton to the horizontal layout
    for (QToolButton *button : buttonList) {
        // Set the tool button style to display text under the icon
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        // Add the button to the horizontal layout
        hLayout->addWidget(button);
    }

    // Add the horizontal layout to the vertical layout
    mainLayout->addLayout(hLayout);

    // Create a label for the group title with bold font
    QLabel *label = new QLabel(groupTitle);
    QFont boldFont = label->font();  // Get the current font
    boldFont.setPointSize(12);
    boldFont.setBold(true);  // Make the font bold
    label->setFont(boldFont);  // Apply the bold font to the label

    // Remove internal margins of the label
    label->setMargin(0);  // Remove internal margin of the label
    label->setAlignment(Qt::AlignCenter);   // Horizontally center the text

    // Add the label to the vertical layout
    mainLayout->addWidget(label);

    // Set the main layout to the QWidget
    widget->setLayout(mainLayout);

    // Add the widget to the toolbar or window
    topToolBar_->addWidget(widget);
}

void MainWindow::setToolBarGroup(QHBoxLayout* ctlWidgetLayout, QString groupTitle)
{
    // Create a new QWidget container
    QWidget *widget = new QWidget();

    // Create a vertical layout to hold all the elements
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Set the spacing for the main vertical layout
    mainLayout->setSpacing(0);  // Remove spacing between widgets
    mainLayout->setContentsMargins(10, 5, 10, 5);  // Set left and right margins (10px), top and bottom 5px

    // Set the spacing for the horizontal layout
    ctlWidgetLayout->setSpacing(3);  // Remove spacing between buttons
    ctlWidgetLayout->setContentsMargins(0, 5, 0, 5);  // No margins around the buttons

    // Add the horizontal layout to the vertical layout
    mainLayout->addLayout(ctlWidgetLayout);

    // Create a label for the group title with bold font
    QLabel *label = new QLabel(groupTitle);
    QFont boldFont = label->font();     // Get the current font
    boldFont.setPointSize(12);
    boldFont.setBold(true);             // Make the font bold
    label->setFont(boldFont);           // Apply the bold font to the label

    // Remove internal margins of the label
    label->setMargin(0);  // Remove internal margin of the label
    label->setAlignment(Qt::AlignCenter);   // Horizontally center the text

    // Add the label to the vertical layout
    mainLayout->addWidget(label);

    // Set the main layout to the QWidget
    widget->setLayout(mainLayout);

    // Add the widget to the toolbar or window
    topToolBar_->addWidget(widget);
}

void MainWindow::setWidgetProgress(int prog) {
    if (!progressWidget_) {
        qWarning() << "Progress widget is not initialized!";
        return;
    }

    // Find the QProgressBar within the widget
    if (!progressBar_) {
        qWarning() << "ProgressBar not found within the progress widget!";
        return;
    }

    if (prog < 100) {
        // Update progress bar and ensure the widget is visible
        progressBar_->setValue(prog);
        if (!progressWidget_->isVisible()) {
            progressWidget_->show();
        }
    } else {
        // If progress is 100, reset and close the widget
        progressBar_->setValue(0);       // Reset progress bar to 0
        progressWidget_->hide();
    }
    QCoreApplication::processEvents();
}


void MainWindow::onAddImgActionTriggered() {
    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Select Sensor Data");
    QString sensorTypeStr = sensorTypeToString(sensorType_);

    // Create widgets
    QLabel *sensorLabel = new QLabel("Sensor Type:", &dialog);
    QLabel *sensorTypeLabel = new QLabel(sensorTypeStr, &dialog);

    QLabel *formatLabel = new QLabel("Data Format:", &dialog);
    QComboBox *formatComboBox = new QComboBox(&dialog);

    QLabel *pathLabel = new QLabel("Data Path:", &dialog);
    QLineEdit *pathLineEdit = new QLineEdit(&dialog);
    QPushButton *browseButton = new QPushButton("Browse", &dialog);

    // Function to update formatComboBox based on selected sensor type
    auto updateFormatComboBox = [&]() {
        formatComboBox->clear();
        if (sensorType_ == SensorType::ImageCamera) {
            formatComboBox->addItems({"JPEG", "PNG", "BMP"});
        } else if (sensorType_ == SensorType::ProfileScanner) {
            formatComboBox->addItems({"YML", "TXT", "JSON"});
        }
    };

    // Initialize formatComboBox
    updateFormatComboBox();

    // Connect browse button to open folder dialog
    connect(browseButton, &QPushButton::clicked, [&]() {
        QString folderPath = QFileDialog::getExistingDirectory(&dialog, 
                                                               "Select Data Folder", 
                                                               QDir::homePath());
        if (!folderPath.isEmpty()) {
            pathLineEdit->setText(folderPath);
        }
    });

    // Create OK and Cancel buttons
    QPushButton *okButton = new QPushButton("OK", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);

    // Connect buttons to dialog actions
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Layout setup
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    QHBoxLayout *sensorLayout = new QHBoxLayout();
    sensorLayout->addWidget(sensorLabel);
    sensorLayout->addWidget(sensorTypeLabel);

    QHBoxLayout *formatLayout = new QHBoxLayout();
    formatLayout->addWidget(formatLabel);
    formatLayout->addWidget(formatComboBox);

    QHBoxLayout *pathLayout = new QHBoxLayout();
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(pathLineEdit);
    pathLayout->addWidget(browseButton);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(sensorLayout);
    mainLayout->addLayout(formatLayout);
    mainLayout->addLayout(pathLayout);
    mainLayout->addLayout(buttonLayout);

    // Execute dialog and handle result
    if (dialog.exec() == QDialog::Accepted) {
        QString dataFormat = formatComboBox->currentText();
        QString folderPath = pathLineEdit->text();

        if (folderPath.isEmpty() || dataFormat.isEmpty()) {
            QMessageBox::warning(this, "Error", "Data path cannot be empty.");
            return;
        } 

        profilesBuffer_.clear();
        std::vector<cv::Point3f> features;
        std::vector<std::string> files;
        ProfileParser profileParser(folderPath.toStdString(), dataFormat.toStdString());            // Create profile parser
        std::function<void(int)> progCallback = [this](int prog) { setWidgetProgress(prog); };      // TODO: Place progCallback function out of mainwindow
        try { 
            // Attempt to parse the profile files and get the points
            profilesBuffer_ = profileParser.parseProfileFiles("profile", progCallback);
            files = profileParser.getFilePaths();
            features = profileParser.parseFeatureFiles("feature_point");
        } catch (const std::exception& e) {
            // If any exception is thrown, log the failure message
            QString w_msg = QString("Load Dataset Failed: %1").arg(e.what());
            QMessageBox::warning(this, "Warning", w_msg);
            logWin_->log(w_msg);
        } catch (...) {
            // Catch any other unexpected exceptions
            QMessageBox::warning(this, "Error", "Profiles/Features data parsing fail.");
            logWin_->log("Load Dataset Failed: Unknown error occurred.");
        }

        // Profile sheets record
        profileSheets_ = parseProfilePointsToProfileSheets(profilesBuffer_, features, profileParser.getProfileType(), files);

        // Initial allocate half of dataset for rotational(translational) component
        if (profileParser.getProfileType() == "Edge")
            initialDataIndex(profileSheets_.size());

        // Set browserWin_ profile preview
        browserWin_->setContentFromPoints(profilesBuffer_);

        // Attempt to plot the points (the first set of points from the buffer)
        viewerWin_->plotPoints(profilesBuffer_[0], false, profileSheets_[0]);
        propertyWin_->writeProfileSheetToProperties(profileSheets_[0], true);

        // Connect the itemSelected signal from DockWidgetBrowser to a lambda function
        // that logs the selected dataset item's index and pose data to the log window.
        disconnect(browserWin_, &DockWidgetBrowser::itemSelected, nullptr, nullptr);    // Disconnect any previous connection
        connect(browserWin_, &DockWidgetBrowser::itemSelected, [this](int index, const QString& text) {
            viewerWin_->plotPoints(profilesBuffer_[index], false, profileSheets_[index]);
            propertyWin_->writeProfileSheetToProperties(profileSheets_[index], true);
            // Index is the number of listwidget sequence (begin from 0). Dataset item = index + 1 = featuresSheet.index + 1
            logWin_->log(QString("Dataset item selected - Index: %1, Pose: %2").arg(index + 1).arg(text));
        });

        // If everything succeeds, log a success message
        logWin_->log("Dataset Load Successfully.");

        // Display the selected options
        QString summary = QString("Sensor Type: %1\nData Format: %2\nData Folder: %3")
                              .arg(sensorTypeStr)
                              .arg(dataFormat)
                              .arg(folderPath);
        QMessageBox::information(this, "Selection Complete", summary);
    }
}

void MainWindow::onAddRobActionTriggered() {
    // 0. Check sensor data buffer first.
    if (profilesBuffer_.empty()) {
        QMessageBox::warning(this, "Warning", "Please load sensor data before robot data input.");
        return;
    }

    // 1. Open a file dialog to select the file
    QString filePath = QFileDialog::getOpenFileName(this, "Open Robot File", "", "Ls Files (*.ls);;Txt Files (*.txt);;All Files (*.*)");

    // If the user cancels the file selection, return
    if (filePath.isEmpty()) {
        return;
    }

    // 2. Extract the file type based on the extension, e.g., .txt or .ls
    QString fileExtension = QFileInfo(filePath).suffix().toLower();

    // 3. Create an instance of FanucParser
    FanucParser parser;
    robDataBuffer_.clear();
    try {
        // 4. Parse the file using the FanucParser
        robDataBuffer_ = parser.parseRobFile(filePath.toStdString(), fileExtension.toStdString());

        // 5. Handle the parsed data (e.g., displaying or processing it)
        browserWin_->setContentFromRobot(robDataBuffer_);

    } catch (const std::exception& e) {
        // 6. Handle exceptions (e.g., file read or parsing errors)
        QMessageBox::warning(this, "Error", "Robot data parsing fail.");
        logWin_->log(QString("Load Robot Dataset Failed: %1").arg(e.what()));
    }  catch (...) {
        // Catch any other unexpected exceptions
        QMessageBox::warning(this, "Error", "Robot data parsing fail.");
        logWin_->log("Load Robot Dataset Failed: Unknown error occurred.");
    }
}

void MainWindow::onSettingButtonReleased() {
    // Create the dialog window
    QDialog *settingsDialog = new QDialog(this);
    settingsDialog->setWindowTitle("Settings");

    // Create a layout for the dialog
    QVBoxLayout *mainLayout0 = new QVBoxLayout(settingsDialog);
    QHBoxLayout *mainLayout = new QHBoxLayout();        // Not include "Confirm" and "Cancel" buttons

    // Calibration Model label and ComboBox
    QLabel *calibModelLabel = new QLabel("Calibration Model:");
    QComboBox *calibModelComboBox = new QComboBox();
    calibModelComboBox->addItem("Sphere");
    calibModelComboBox->addItem("Edge");

    // Create a horizontal layout to place the label and combo box next to each other
    QHBoxLayout *calibModelLayout = new QHBoxLayout();
    calibModelLayout->addWidget(calibModelLabel);
    calibModelLayout->addWidget(calibModelComboBox);

    // Set initial "CalibrationModel" value based on calibMap_
    if (calibMap_.contains("CalibrationModel")) {
        QString calibModel = QString::fromStdString(calibMap_["CalibrationModel"]);
        if (calibModel == "Sphere") {
            calibModelComboBox->setCurrentIndex(0);
        } else if (calibModel == "Edge") {
            calibModelComboBox->setCurrentIndex(1);
        }
    } 
    else {
        calibMap_["CalibrationModel"] = "Sphere";
        calibModelComboBox->setCurrentIndex(0);
    }

    // Create a layout for the image (on the top)
    QGroupBox *imageGBox = new QGroupBox("Model Configuration");
    updateCalibModelGroupBox(imageGBox, calibMap_);

    QVBoxLayout *modelLayout = new QVBoxLayout();
    modelLayout->addLayout(calibModelLayout);
    modelLayout->addWidget(imageGBox);

    // Create algorithm "configGroutBox".
    QVBoxLayout *propertiesLayout = new QVBoxLayout();
    QGroupBox *configGroupBox = new QGroupBox("Algorithm Setting"); 

    // Add the horizontal layout to the properties layout
    propertiesLayout->addWidget(configGroupBox);
    propertiesLayout->setSpacing(10);

    // Add the image layout and properties layout to the main layout
    mainLayout->addLayout(modelLayout);
    mainLayout->addLayout(propertiesLayout);

    // Create a layout for the buttons (Confirm and Cancel)
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    QPushButton *confirmButton = new QPushButton("Confirm");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    // Add button layout to the main layout, it will be placed at the bottom of the dialog
    mainLayout0->addLayout(mainLayout);
    mainLayout0->addLayout(buttonLayout);

    // Set the dialog's layout
    settingsDialog->setLayout(mainLayout);

    // Connect the calibModelComboBox signal to update calibMap_["CalibrationModel"]
    connect(calibModelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index) {
            // Update calibMap_["CalibrationModel"] based on the selected item
            if (index == 0) {
                calibMap_["CalibrationModel"] = "Sphere";
            } else if (index == 1) {
                calibMap_["CalibrationModel"] = "Edge";
            }
            logWin_->log("Calibration Model set to: " + QString::fromStdString(calibMap_["CalibrationModel"]));
            updateCalibModelGroupBox(imageGBox, calibMap_);
    });

    // Connect the confirm button to process data
    connect(confirmButton, &QPushButton::clicked, [=]() {
        settingsDialog->accept();       // Close the dialog

        // Proceed with data processing
        if (!profilesBuffer_.empty() && calibMap_["CalibrationModel"] == "Sphere" 
            && calibMap_.contains("FeaturePointDirection") && calibMap_.contains("SphereRadius")) 
        {
            auto profile_lines = convertPointsSetBuffer(profilesBuffer_);
            DataProc proc(profile_lines, CalibObj::SPHERE);

            // Load sphere parameters.
            float rad_sphere = calibMap_["SphereRadius"];

            // TODO: Show processing
            std::vector<cv::Point3f> ctr_pnts = proc.CalcSphereCtrs(rad_sphere, calibMap_["FeaturePointDirection"]);
            writeFeaturePointsToProfileSheets(ctr_pnts, "Sphere", profileSheets_);
            logWin_->log("Using Sphere Calibration Model");
        }
        else if (!profilesBuffer_.empty() && calibMap_["CalibrationModel"] == "Edge") {
            logWin_->log("Using Edge Calibration Model");
        }
        else {
            QMessageBox::warning(this, "Error", "Profiles dataset has not been uploaded.");
            logWin_->log("No profiles data detected.");
        }
    });

    // Connect the cancel button to close the dialog without processing
    connect(cancelButton, &QPushButton::clicked, [=]() {
        settingsDialog->reject();  // Close the dialog without processing
    });

    // Set the minimum size for the dialog (adjust as needed)
    settingsDialog->setMinimumSize(600, 300); // Adjust minimum size according to content

    // Show the dialog
    settingsDialog->exec();
}

void MainWindow::onRunButtonReleased() {
    // Check calibration dataset
    if (profilesBuffer_.empty() || robDataBuffer_.empty()) {
        QMessageBox::warning(this, "Error", "Calibration data has not been uploaded.");
        logWin_->log("No calibration data detected.");
        return;
    }
    if (profilesBuffer_.size() != robDataBuffer_.size()) {
        QMessageBox::warning(this, "Error", "The size of calibration data does not match.");
        logWin_->log("Mismatch detected in calibration data sizes.");
        return;
    }

    // Process calibration dataset
    auto xyzwpr_data = convertRobDataBuffer(robDataBuffer_);

    // Execute calibration
	ProfileScanner::HandEyeCalib hec;
    CalibType type = calibMap_["CalibType"] == "Eye-In-Hand" ? CalibType::EYE_IN_HAND : CalibType::EYE_TO_HAND;
    hec.SetCalibType(type);
	hec.SetRobPose(xyzwpr_data);

    if (calibMap_.contains("CalibrationModel")) {
        std::vector<cv::Point3f> fea_points = extractFeaturePointsFromProfileSheet(profileSheets_);

        CalibObj cal_model;
        if (calibMap_["CalibrationModel"] == "Sphere")
            cal_model = CalibObj::SPHERE;
        else if (calibMap_["CalibrationModel"] == "Edge")
            cal_model = CalibObj::EDGE;

        hec.SetProfileData(fea_points, cal_model, dataIndex_);
    }
    else {
        QMessageBox::warning(this, "Warning", "Unknown Calibration Model.");
        logWin_->log("Unknown Calibration Model.");
        return;
    }
    
	hec.run(ProfileScanner::SolveMethod::ITERATION);
    // float calib_error = hec.CalcCalibError();

    // // Log the results
    // std::ostringstream logStream;
    // logStream << "Calibration Error: " << calib_error;
    // logWin_->log(QString::fromStdString(logStream.str()));

}

void MainWindow::showScanCameraDialog(QAction *actBtn) {
    // Create a dialog to set camera connect
    QDialog dialog(this);
    dialog.setWindowTitle("Scan Cameras");
    dialog.setMinimumWidth(500);
    dialog.setMinimumHeight(220);

    QVBoxLayout *layout = new QVBoxLayout(&dialog); 
    layout->setContentsMargins(10, 10, 10, 10); 
    layout->setAlignment(Qt::AlignCenter);
    dialog.setLayout(layout);

    // Create a combo box for selecting sensor brand
    QHBoxLayout *brandLayout = new QHBoxLayout(&dialog);
    QLabel *brandLabel = new QLabel("Sensor brand:", &dialog);
    brandLayout->addWidget(brandLabel);

    QComboBox *brandComboBox = new QComboBox(&dialog);
    brandLayout->addWidget(brandComboBox, 1);
    brandComboBox->addItem(" "); 
    brandComboBox->addItem("LMI"); 
    brandComboBox->addItem("SSZN");
    layout->addLayout(brandLayout);

    // Create a combo box for selecting camera IDs
    QHBoxLayout *infoLayout = new QHBoxLayout(&dialog);
    QLabel *infoLabel = new QLabel("Select Sensor:", &dialog);
    infoLayout->addWidget(infoLabel);
    
    QComboBox *cameraComboBox = new QComboBox(&dialog);
    infoLayout->addWidget(cameraComboBox, 1);       // Add combo box to layout
    layout->addLayout(infoLayout);
    
    // Initial with configured sensor
    if (curCamInfo_.id != -1){
        // Check the brand of the current camera info
        if (curCamInfo_.brand == "LMI")
            brandComboBox->setCurrentIndex(1);  // Set the first item (LMI) as the selected item
        else if (curCamInfo_.brand == "SSZN")
            brandComboBox->setCurrentIndex(2);  // Set the second item (SSZN) as the selected item
        else {
            logWin_->log("Error: Unknown sensor brand.");
            brandComboBox->setCurrentIndex(0);
        }
        cameraComboBox->addItem(QString("%1 - %2").arg(curCamInfo_.id)
                    .arg(QString::fromStdString(curCamInfo_.ipAddress)));
    }
    // Initial action button status
    actBtn->setChecked(curCamInfo_.isConnected);

    // Scan sensor button
    QPushButton *scanButton = new QPushButton("Scan", &dialog); 
    layout->addWidget(scanButton);

    // Create a group box for camera status
    QGroupBox *statusGroupBox = new QGroupBox("Current Sensor", &dialog); 
    updateSeneorInfoGroupBox(statusGroupBox, curCamInfo_);      // Update Sensor Group Box Info
    layout->addWidget(statusGroupBox);

    // Create a horizontal layout for connect and disconnect buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *connectButton = new QPushButton("Connect", &dialog);
    buttonLayout->addWidget(connectButton);
    QPushButton *disconnectButton = new QPushButton("Disconnect", &dialog);
    buttonLayout->addWidget(disconnectButton);
    layout->addLayout(buttonLayout);    // Add horizontal layout to main layout

    // Create a label for displaying camera information
    QLabel *cameraInfoLabel = new QLabel(&dialog); // Define camera info label
    layout->addWidget(cameraInfoLabel); 

    connect(brandComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
        [=]() {
            QString selectedItem = brandComboBox->currentText();
            curCamInfo_.brand = selectedItem.toStdString();
            updateSeneorInfoGroupBox(statusGroupBox, curCamInfo_);
            logWin_->log("Selected brand: " + selectedItem);
    });

    connect(scanButton, &QPushButton::clicked, [=]() {
        std::vector<CameraInfo> sensorList;
        // Scan the cameras and update the sensor list
        sensorApi_.SetBrand(curCamInfo_.brand);     // Initial set brand
        CameraStatus status = sensorApi_.Scan(sensorList);
        if (status == CameraStatus::DEV_READY) {
            // Populate the combo box with camera info if scan is successful
            cameraComboBox->clear();    // Clear previous entries
            for (const auto& camera : sensorList) {
                cameraComboBox->addItem(QString("%1 - %2").arg(camera.id)
                    .arg(QString::fromStdString(camera.ipAddress)));
            }

            // Update curCamInfo with the first camera's information
            if (!sensorList.empty()) {
                curCamInfo_.ipAddress = sensorList[0].ipAddress; // Update IP address
                curCamInfo_.id = sensorList[0].id;               // Update camera ID
                // Optionally, you can set curCamInfo.brand if brand information is available
                curCamInfo_.brand = sensorList[0].brand;         // Update camera brand if needed
            } else {
                QMessageBox::warning(this, "Warning", "No device detected.");
                return;
            }

            // Update status and camera ID labels
            updateSeneorInfoGroupBox(statusGroupBox, curCamInfo_);
            cameraInfoLabel->setText("Cameras scanned successfully."); // Update camera info label

            // Connect the combo box selection change to update curCamInfo
            connect(cameraComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, sensorList](int index) {
                if (index >= 0 && index < sensorList.size()) {
                    // Update curCamInfo based on the selected camera
                    curCamInfo_.ipAddress = sensorList[index].ipAddress; // Update IP address
                    curCamInfo_.id = sensorList[index].id;               // Update camera ID
                    curCamInfo_.brand = sensorList[index].brand;         // Update camera brand if needed
                    // Optionally, update labels or other UI elements here
                }
            });
        } else {
            cameraInfoLabel->setText("Failed to scan cameras.");
        }
    });

    // Connect the buttons to slot functions, including connect and disconnect.
    connect(connectButton, &QPushButton::clicked, [=]() {
        // Create a new thread for camera grabbing
        grabThread_ = new QThread(this); 
        grabWorker_ = new ThreadWorker(&sensorApi_);    // Grab data cache in sensorApi_
        grabWorker_->moveToThread(grabThread_);
        connect(grabThread_, &QThread::started, grabWorker_, &ThreadWorker::startGrabbing);
        connect(grabWorker_, &ThreadWorker::updatePlot, this, &MainWindow::replotSensorData);

        // Check if the IP address is valid (not empty)
        if (curCamInfo_.ipAddress.empty() || curCamInfo_.ipAddress == "0.0.0.0") {
            // Show warning message if IP address is not set
            QMessageBox::warning(this, "Warning", "Please select a valid camera IP address before connecting.");
            return;
        }

        // Attempt to connect to the camera
        CameraStatus status = sensorApi_.Connect(curCamInfo_.ipAddress);
        if (status == CameraStatus::DEV_READY) {
            // Update connection status and brand label if successful
            curCamInfo_.isConnected = true;
            updateSeneorInfoGroupBox(statusGroupBox, curCamInfo_);
            QString logMsg = QString("Info: Connected to camera: %1").arg(QString::fromStdString(curCamInfo_.ipAddress));
            logWin_->log(logMsg);
            actBtn->setChecked(true);                   // Set menu QAction status true if camera connected.

            // Emit signal to enable DockWidgetViewer buttons and connect sensor slot
            emit sensorConnStatue(true);
            connect(viewerWin_->getButtonList()[0], &QPushButton::toggled, this, &MainWindow::onPlayToggled);
            connect(viewerWin_->getButtonList()[1], &QPushButton::clicked, this, &MainWindow::onCaptureClicked);
        } else {
            // Show error message if connection fails
            QMessageBox::critical(this, "Error", "Failed to connect to the camera.");
        }
    });

    connect(disconnectButton, &QPushButton::clicked, [=]() {
        // Attempt to disconnect to the camera
        CameraStatus status = sensorApi_.Disconnect(curCamInfo_.ipAddress);
        if (status == CameraStatus::DEV_NOT_CONNECTED) {
            // Update connection status and brand label if disconnect successful
            curCamInfo_.isConnected = false;
            updateSeneorInfoGroupBox(statusGroupBox, curCamInfo_);
            QString logMsg = QString("Info: Disconnected to camera: %1").arg(QString::fromStdString(curCamInfo_.ipAddress));
            logWin_->log(logMsg);
            actBtn->setChecked(false);                  // Set menu QAction status

            // Emit signal to disable DockWidgetViewer buttons and disconnect sensor slot
            emit sensorConnStatue(false);
            disconnect(viewerWin_->getButtonList()[0], &QPushButton::toggled, this, &MainWindow::onPlayToggled);
            disconnect(viewerWin_->getButtonList()[1], &QPushButton::clicked, this, &MainWindow::onCaptureClicked);

            // Stop the grabbing process
            grabWorker_->stopGrabbing();  // Ensure the worker stops grabbing
            grabThread_->quit();
            grabThread_->wait();  // Blocks until the thread finishes

            // Stop and destroy grab thread
            delete grabThread_;
            delete grabWorker_;
        } else {
            // Show error message if connection fails
            QMessageBox::critical(this, "Error", "Failed to disconnect to the camera.");
        }
    });

    dialog.exec();
}

void MainWindow::updateSeneorInfoGroupBox(QGroupBox *statusGroupBox, const CameraInfo &curCamInfo_) {
    // Clear all items in the old layout
    QLayout *oldLayout = statusGroupBox->layout();
    if (oldLayout) {
        QWidgetList widgets = statusGroupBox->findChildren<QWidget*>();
        for (QWidget* widget : widgets) {
            widget->deleteLater(); // Delete widgets from the layout
        }
        delete oldLayout;
    } 
    // Create a new vertical layout for the group box
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroupBox);

    // Create a label for displaying current camera brand
    QLabel *brandInfo = new QLabel("Camera Brand: " + 
        QString::fromStdString(curCamInfo_.brand), statusGroupBox); // Initial brand label
    statusLayout->addWidget(brandInfo);        // Add brand label to group box layout

    // Create a horizontal layout for status and camera ID
    QString id_s = (curCamInfo_.id == -1) ? "N/A" : QString::number(curCamInfo_.id);
    QString conn_s = (curCamInfo_.isConnected) ? "Connected" : "Not Connected";
    QHBoxLayout *statusIdLayout = new QHBoxLayout();
    QLabel *statusLabel = new QLabel("Status: " + conn_s, statusGroupBox); // Initial status
    QLabel *cameraIdLabel = new QLabel("Camera ID: " + id_s, statusGroupBox); // Initial camera ID

    statusIdLayout->addWidget(statusLabel);     // Add status label to horizontal layout
    statusIdLayout->addWidget(cameraIdLabel);   // Add camera ID label to horizontal layout
    statusLayout->addLayout(statusIdLayout);    // Add horizontal layout to group box layout

    // Apply the new layout to the group box
    statusGroupBox->setLayout(statusLayout);  // Make sure to set the new layout
}

void MainWindow::updateCalibModelGroupBox(QGroupBox *modelGroupBox, nlohmann::json &calib_map) {
    // Clear all items in the old layout
    QLayout *oldLayout = modelGroupBox->layout();
    if (oldLayout) {
        QWidgetList widgets = modelGroupBox->findChildren<QWidget*>();
        for (QWidget* widget : widgets) {
            widget->deleteLater(); // Delete widgets from the layout
        }
        delete oldLayout;
    } 
    // Create a new vertical layout for the group box
    QVBoxLayout *modelLayout = new QVBoxLayout(modelGroupBox);

    // Try to get calibration model from map.
    QString calibModel = QString();
    if (calibMap_.contains("CalibrationModel"))
        calibModel = QString::fromStdString(calibMap_["CalibrationModel"]);

    // Load the image and scale it to fit within the fixed square size
    QLabel *imageLabel = new QLabel(); 
    QString qrcImgName = ":/images/" + calibModel + ".png";
    QPixmap imagePixmap(qrcImgName);
    imageLabel->setPixmap(imagePixmap.scaledToHeight(300, Qt::SmoothTransformation));  // Scale image to fit
    // imageLabel->setFixedSize(300, 300);     // Ensure imageLabel is always 300x300px
    imageLabel->setStyleSheet(
        "background-color:rgb(200, 200, 200);"
        "color: #444444;"
        "border: 2px solid #666666;"
    );
    imageLabel->setAlignment(Qt::AlignCenter);
    

    // Create a horizontal layout to place the label and combo box next to each other
    // Feature Point Direction label and ComboBox
    QVBoxLayout *configLayout = new QVBoxLayout();

    if (calibModel == "Sphere") {
        QHBoxLayout *featureDirectionLayout = new QHBoxLayout();
        QLabel *featureDirectionLabel = new QLabel("Feature Point Direction:");
        QComboBox *featureDirectionComboBox = new QComboBox();
        featureDirectionComboBox->addItem("+Y");
        featureDirectionComboBox->addItem("-Y");
        featureDirectionLayout->addWidget(featureDirectionLabel);
        featureDirectionLayout->addWidget(featureDirectionComboBox);
        configLayout->addLayout(featureDirectionLayout);
    
        QHBoxLayout *sphereRadiusLayout = new QHBoxLayout();
        QLabel *sphereRadiusLabel = new QLabel("Sphere Radius:");
        QSpinBox *sphereRadiusSpinBox = new QSpinBox();
        sphereRadiusSpinBox->setRange(1, 100);
        sphereRadiusSpinBox->setSuffix(" mm");
        sphereRadiusLayout->addWidget(sphereRadiusLabel);
        sphereRadiusLayout->addWidget(sphereRadiusSpinBox);
        configLayout->addLayout(sphereRadiusLayout);

        // Initial UI with calib_map, or set default calib_map value
        if (calib_map.contains("FeaturePointDirection")) {
            QString featureDirection = QString::fromStdString(calib_map["FeaturePointDirection"]);
            if (featureDirection == "+Y") {
                featureDirectionComboBox->setCurrentIndex(0);
            } else if (featureDirection == "-Y") {
                featureDirectionComboBox->setCurrentIndex(1);
            }
        }
        else {
            calib_map["FeaturePointDirection"] = "+Y";
            featureDirectionComboBox->setCurrentIndex(0);
        }

        if (calib_map.contains("SphereRadius")) {
            sphereRadiusSpinBox->setValue(calib_map["SphereRadius"]);
        }
        else {
            calib_map["SphereRadius"] = 40;
            sphereRadiusSpinBox->setValue(calib_map["SphereRadius"]);
        }

        // Connect the featureDirectionComboBox signal to update calibMap_["FeaturePointDirection"]
        connect(featureDirectionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index) {
                // Update calibMap_["FeaturePointDirection"] based on the selected item
                if (index == 0) {
                    calibMap_["FeaturePointDirection"] = "+Y";
                } else if (index == 1) {
                    calibMap_["FeaturePointDirection"] = "-Y";
                }
                logWin_->log("Feature Point Direction set to: " + QString::fromStdString(calibMap_["FeaturePointDirection"]));
        });

        // Connect valueChanged signal to an anonymous lambda function
        connect(sphereRadiusSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            [=](int value) {
                calibMap_["SphereRadius"] = value;  // Store value as an integer
                logWin_->log("Sphere Radius set to: " + QString::number(value) + " mm");
        });
    }
    else if (calibModel == "Edge") {
        QHBoxLayout *datasetLayout = new QHBoxLayout();
        QLabel *datasetLabel = new QLabel("Rotational component (Number of dataset):");
        QSpinBox *datasetSpinBox = new QSpinBox();
        datasetSpinBox->setRange(0, 100);
        datasetSpinBox->setSuffix(" set");
        datasetSpinBox->setValue(getDataIndexAlloc(dataIndex_).first);
        datasetLayout->addWidget(datasetLabel);
        datasetLayout->addWidget(datasetSpinBox);

        QHBoxLayout *datasetLayout1 = new QHBoxLayout();
        QLabel *datasetLabel1 = new QLabel("Translational component (Number of dataset):");
        QSpinBox *datasetSpinBox1 = new QSpinBox();
        datasetSpinBox1->setRange(0, 100);
        datasetSpinBox1->setSuffix(" set");
        datasetSpinBox1->setValue(getDataIndexAlloc(dataIndex_).second);
        datasetLayout1->addWidget(datasetLabel1);
        datasetLayout1->addWidget(datasetSpinBox1);

        configLayout->addLayout(datasetLayout);
        configLayout->addLayout(datasetLayout1);
        
        // Update dataIndex_ for calibrate rotational and translational component
        connect(datasetSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            [=](int value){
                std::fill(dataIndex_.begin(), dataIndex_.begin() + value, 0);
                std::fill(dataIndex_.begin() + value, dataIndex_.end(), 1);
                datasetSpinBox1->setValue(profileSheets_.size() - value);
        });

        connect(datasetSpinBox1, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int value){
                std::fill(dataIndex_.end() - value, dataIndex_.end(), 1);
                std::fill(dataIndex_.begin(), dataIndex_.end() - value, 0);
                datasetSpinBox->setValue(profileSheets_.size() - value);
        });
    }

    // Add the image label to the image layout
    modelLayout->addWidget(imageLabel);
    modelLayout->addLayout(configLayout);

    // Apply the new layout to the group box
    modelGroupBox->setLayout(modelLayout);
}

void MainWindow::onPlayToggled(bool ckecked) {
    // Check sensor connect first
    if (!curCamInfo_.isConnected) {
        QMessageBox::warning(this, "Input Error", "Please connect sensor before open it.");
        return;
    }

    // Check the current icon and toggle between play and pause icons
    if (ckecked) {
        // Start the thread, triggering ContinueGrabPlot, sensor shouble open first
        try
        {
            sensorApi_.SetStatus(true);
        }
        catch(const std::exception& e){
            // BUG: 
            QString msg = QString("Open Sensor Fail: %1").arg(e.what());
            QMessageBox::warning(this, "Error", msg);
            return;
        }
        
        grabThread_->start();

        // Switch to pause icon
        viewerWin_->getButtonList()[0]->setIcon(QIcon(":/icons/pause.png"));
        viewerWin_->getButtonList()[0]->setToolTip("Pause camera grab");
        isGrabing_ = true;
    } 
    else {
        // Switch to play icon
        viewerWin_->getButtonList()[0]->setIcon(QIcon(":/icons/play.png"));
        viewerWin_->getButtonList()[0]->setToolTip("Run camera grab");

        // isPlay set to false to stop thread loop
        isGrabing_ = false;
        grabWorker_->stopGrabbing();
        grabThread_->quit();
        
        sensorApi_.SetStatus(false);  // Stop the camera
    }
}

void MainWindow::onCaptureClicked() {
    sensorApi_.SetStatus(true);
    sensorApi_.GrabOnce();
    RenderData profile = convertToRenderData(sensorApi_.RetriveData());
    ProfileSheet sheet;
    viewerWin_->plotPoints(profile, false, sheet);
}

void MainWindow::replotSensorData() {
    // Step 1: Retrieve data from the sensor
    auto sensorData = sensorApi_.RetriveData();

    // Step 2: Convert the data to the renderable format
    RenderData profile = convertToRenderData(sensorData);
    
    // Step 3: Prepare the ProfileSheet (assuming it contains relevant display settings)
    ProfileSheet sheet;
    
    // Step 4: Plot the points on the viewer window
    viewerWin_->plotPoints(profile, false, sheet, false);
    
    // Step 5: Refresh the viewer window (or any relevant UI element)
    viewerWin_->update();
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

// Function to convert SensorType to string
QString MainWindow::sensorTypeToString(SensorType sensorType) {
    switch (sensorType) {
        case SensorType::ProfileScanner:
            return "Profile Scanner";
        case SensorType::ImageCamera:
            return "Image Camera";
        default:
            return "Unknown Sensor Type";  // Default case
    }
}

// Convert pointsSetBuffer_ to std::vector<std::vector<cv::Point3f>>
std::vector<std::vector<cv::Point3f>> MainWindow::convertPointsSetBuffer(const std::vector<RenderData>& pointsSetBuffer) {
    std::vector<std::vector<cv::Point3f>> result;

    for (const auto& profile : pointsSetBuffer) {
        std::vector<cv::Point3f> profilePoints;
        for (const auto& point : profile) {
            profilePoints.emplace_back(cv::Point3f(static_cast<float>(point.first),
                                                   0.0f,
                                                   static_cast<float>(point.second)));
        }
        result.emplace_back(std::move(profilePoints));
    }

    return result;
}


// Convert robDataBuffer_ to std::vector<Eigen::Vector<float, 6>>
std::vector<Eigen::Vector<float, 6>> MainWindow::convertRobDataBuffer(const std::vector<FanucRobPose>& robDataBuffer) {
    std::vector<Eigen::Vector<float, 6>> result;

    for (const auto& pose : robDataBuffer) {
        if (pose.size() != 6) {
            throw std::runtime_error("Invalid FanucRobPose size. Expected 6 elements.");
        }

        Eigen::Vector<float, 6> vector;
        for (size_t i = 0; i < 6; ++i) {
            vector[i] = static_cast<float>(pose[i]);
        }

        result.emplace_back(vector);
    }

    return result;
}

// Function to RenderData to ProfileSheet
std::vector<ProfileSheet> MainWindow::parseProfilePointsToProfileSheets(
    const std::vector<RenderData>& pointsSetBuffer, 
    std::vector<cv::Point3f> features,
    std::string feature_type,
    std::vector<std::string> paths) 
{
    std::vector<ProfileSheet> featuresSheet;

    // Iterate through the pointsSetBuffer to create ProfileSheet objects
    for (size_t i = 0; i < pointsSetBuffer.size(); ++i) {
        const RenderData& points = pointsSetBuffer[i];

        // Create a ProfileSheet for each RenderData
        ProfileSheet profileSheet;
        profileSheet.profileIndex = static_cast<int>(i);            // Set the profile index
        profileSheet.pointCount = static_cast<int>(points.size());  // Set the number of points in the profile
        profileSheet.file_path = paths[i];
        profileSheet.type = feature_type;
        profileSheet.enableFilter = false;                          // Set the filter flag (can be adjusted)
        profileSheet.filterType = "";                               // Set filter type (can be adjusted)

        if (!features.empty())
            profileSheet.featurePoint = features[i];                    // Leave featurePoint empty or set to a default value (0,0,0)
        else
            profileSheet.featurePoint = {0, 0, 0};

        // Add the profileSheet to the featuresSheet vector
        featuresSheet.push_back(profileSheet);
    }

    return featuresSheet;  // Return the vector of ProfileSheet objects
}

std::vector<cv::Point3f> MainWindow::extractFeaturePointsFromProfileSheet(const std::vector<ProfileSheet>& profileSheets) {
    std::vector<cv::Point3f> featurePoints;

    // Loop through the profileSheets vector and extract feature points
    for (const auto& profile : profileSheets) {
        featurePoints.push_back(profile.featurePoint);  // Extract the feature point and add to the result vector
    }

    return featurePoints;  // Return the vector containing all extracted feature points
}

// Function to write cv::Point3f data into ProfileSheet objects
void MainWindow::writeFeaturePointsToProfileSheets(const std::vector<cv::Point3f>& points, std::string type, std::vector<ProfileSheet>& profileSheets) {
    // Check if the profileSheets is empty
    if (profileSheets.empty()) {
        // If it's empty, push new ProfileSheet objects for each point in 'points'
        for (size_t i = 0; i < points.size(); ++i) {
            ProfileSheet profile;
            profile.profileIndex = static_cast<int>(i);  // Set profile index
            profile.pointCount = 1;                      // Set point count to 1 for each feature point
            profile.featurePoint = points[i];            // Set the feature point
            profile.filterType = type;                   
            profile.enableFilter = false;                // Set the filter status (can be adjusted based on your logic)
            profile.filterType = "";                     // Set the filter type (can be adjusted based on your logic)

            // Add the created ProfileSheet to the profileSheets vector
            profileSheets.push_back(profile);
        }
    } else {
        // If profileSheets already has elements, update them (assuming sizes match)
        if (profileSheets.size() == points.size()) {
            for (size_t i = 0; i < points.size(); ++i) {
                profileSheets[i].featurePoint = points[i];  // Directly update the feature point
                profileSheets[i].type = type;     
            }
        } else {
            // Handle the case where the sizes don't match (you could choose to clear and rewrite, or handle the mismatch)
            logWin_->log("Error: The number of points does not match the size of profileSheets.");
        }
    }
}

// Function to project 3D points onto the X-OZ plane (ignore Y coordinate)
std::pair<double, double> MainWindow::projectToXozPlane(const cv::Point3f& point) {
    std::pair<double, double> projectedPoint;


    // Discard the Y coordinate and keep X and Z for projection onto X-OZ plane
    projectedPoint = std::make_pair(static_cast<double>(point.x), static_cast<double>(point.z));

    // Return the vector of projected 2D points (X, Z)
    return projectedPoint;
}

void MainWindow::replaceProfileSheet(std::vector<ProfileSheet>& profiles, const ProfileSheet& newProfile) {
    // Find the ProfileSheet in profiles with the same profileIndex as newProfile
    auto it = std::find_if(profiles.begin(), profiles.end(), [&newProfile](const ProfileSheet& profile) {
        return profile.profileIndex == newProfile.profileIndex;
    });

    // If a matching profile is found, replace it
    if (it != profiles.end()) {
        *it = newProfile;  // Replace the old profile with the new one
    } else {
        // If no matching profile is found, you can throw an exception or handle the case
        throw std::out_of_range("Profile with given profileIndex not found!");
    }
}

void MainWindow::saveProfileToFile(const RenderData& profile, const ProfileSheet& sheet) {
    // Open the YML file for writing
    cv::FileStorage fs(sheet.file_path, cv::FileStorage::WRITE);

    // Check if file opened successfully
    if (!fs.isOpened()) {
        std::cerr << "Failed to open file: " << sheet.file_path << std::endl;
        return;
    }

    // Convert ProfilePoints to a flat list of x, y, z values
    std::vector<double> profileData;
    for (const auto& point : profile) {
        profileData.push_back(point.first);  // x-coordinate
        profileData.push_back(0.0);          // y-coordinate (set to 0 as per requirement)
        profileData.push_back(point.second); // z-coordinate
    }

    // Write profile data to the YML file
    fs << "profile" << profileData;

    // Write feature point to the YML file
    std::vector<double> featurePoint = {sheet.featurePoint.x, sheet.featurePoint.y, sheet.featurePoint.z};
    fs << "feature_point" << featurePoint;

    fs << "type" << "\"" + sheet.type + "\"";

    // Close the file after writing
    fs.release();
}

std::vector<std::pair<double, double>> MainWindow::convertToRenderData(const ProfileData& data) {
    std::vector<std::pair<double, double>> profilePoints;
    
    // Ensure there are valid points in the buffer
    if (data.profileBuffer == nullptr || data.validPoints == 0) {
        return profilePoints; // Return empty vector if no valid points
    }

    // Extract the x, z coordinates from valid profile points
    for (size_t i = 0; i < data.validPoints; ++i) {
        const ProfilePoint& point = data.profileBuffer[i];
        profilePoints.emplace_back(point.x, point.z); // Add the (x, z) pair to the vector
    }

    return profilePoints;
}

void MainWindow::initialDataIndex(size_t number_dataset) {
    dataIndex_.resize(number_dataset);
    std::fill(dataIndex_.begin(), dataIndex_.begin() + number_dataset/2, 0);
    std::fill(dataIndex_.begin() + number_dataset/2, dataIndex_.end(), 1);
}

std::pair<int, int> MainWindow::getDataIndexAlloc(std::vector<int> dataIndex) {
    std::pair<int, int> number_rt = {0, 0};
    for (size_t i = 0; i < dataIndex.size(); i++)
    {
        if (dataIndex[i] == 0)
            number_rt.first ++;
        else if (dataIndex[i] == 1)
            number_rt.second ++;
    }
    return number_rt;
}