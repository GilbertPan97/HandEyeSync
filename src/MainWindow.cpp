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
    viewerWin_ = new DockWidgetViewer("Viewer", this);
    logWin_ = new DockWidgetLogger("Logger", this);
    browserWin_ = new DockWidgetBrowser("Data Browser", this);
    propertyWin_ = new DockWidgetProperty("Property Browser", this);

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

    dockWidgets_ << viewerWin_ << logWin_ << browserWin_;

    // Adjust layout margins to remove any gaps
    centralLayout->setContentsMargins(0, 0, 0, 0);  // No margins between widgets
    centralLayout->addWidget(dockManager_);

    setCentralWidget(centralWidget);  // Set it as the central widget

    // Create menu and toolbar by calling the functions
    createMenuBar();
    createToolBar();

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
    
    QHBoxLayout *collectLayout = new QHBoxLayout(this);
    QLabel *indexLabel = new QLabel("Index", this);
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItem("0");
    QPushButton *downloadButton = new QPushButton(this);
    downloadButton->setIcon(QIcon(":/icons/download.png"));
    downloadButton->setToolTip("Collect");
    collectLayout->addWidget(indexLabel, 0, Qt::AlignVCenter);
    collectLayout->addWidget(comboBox, 0, Qt::AlignVCenter);
    collectLayout->addWidget(downloadButton, 0, Qt::AlignVCenter);

    onlineCollectLayout->addWidget(onlineCollectBtn);
    onlineCollectLayout->addLayout(collectLayout);

    QMenu *addImgMenu = new QMenu(this);
    QAction *addImage1Action = addImgMenu->addAction("Local Images");
    QAction *addImage2Action = addImgMenu->addAction("Receive From Camera");
    connect(addImage1Action, &QAction::triggered, this, &MainWindow::onAddImg1ActionTriggered);
    connect(addImage2Action, &QAction::triggered, this, &MainWindow::onAddImg2ActionTriggered);

    QToolButton *addImgButton = new QToolButton();
    addImgButton->setIcon(QIcon(":/icons/add-images.png"));
    addImgButton->setText("Add Images");
    addImgButton->setMenu(addImgMenu);
    addImgButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addImgButton->setPopupMode(QToolButton::MenuButtonPopup);

    QMenu *addRobMenu = new QMenu(this);
    QAction *addRob1Action = addRobMenu->addAction("Local Robot Data");
    QAction *addRob2Action = addRobMenu->addAction("Receive From Robot");
    // Connect the QAction signals to the slot functions
    connect(addRob1Action, &QAction::triggered, this, &MainWindow::onAddRob1ActionTriggered);
    connect(addRob2Action, &QAction::triggered, this, &MainWindow::onAddRob2ActionTriggered);

    QToolButton *addRobButton = new QToolButton();
    addRobButton->setIcon(QIcon(":/icons/add-robot.png"));
    addRobButton->setText("Add Robot Data");
    addRobButton->setMenu(addRobMenu);
    addRobButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addRobButton->setPopupMode(QToolButton::MenuButtonPopup);

    QHBoxLayout *datasetLayout = new QHBoxLayout(this);
    datasetLayout->addLayout(onlineCollectLayout);
    datasetLayout->addWidget(addImgButton);
    datasetLayout->addWidget(addRobButton);

    // Group the buttons using setToolBarGroup
    QList<QToolButton*> data_btnList = { addImgButton, addRobButton };
    setToolBarGroup(datasetLayout, "DATASET");

    topToolBar_->addSeparator();

    /* ========================= Calibrate Page ========================= */
    QVBoxLayout* calibModelLayout = new QVBoxLayout(this);
    calibModelLayout->setSpacing(10);
    QLabel *calibModelLabel = new QLabel("Calibration Model:");
    calibModelLayout->addWidget(calibModelLabel);
    QRadioButton *eyeInHandRadioButton = new QRadioButton("Eye-In-Hand");
    QRadioButton *eyeToHandRadioButton = new QRadioButton("Eye-To-Hand");

    // Create a button group to ensure only one can be selected at a time
    QButtonGroup *modelGroup = new QButtonGroup(this);
    modelGroup->addButton(eyeInHandRadioButton);
    modelGroup->addButton(eyeToHandRadioButton);

    // Add the radio buttons to the layout
    calibModelLayout->addWidget(eyeInHandRadioButton);
    calibModelLayout->addWidget(eyeToHandRadioButton);
    eyeInHandRadioButton->setChecked(true);         // Set a default selection

    // Calibrate setting and run button
    QMenu *calSettingMenu = new QMenu(this);
    QAction *calSet1Action = calSettingMenu->addAction("Method");
    QAction *calSet2Action = calSettingMenu->addAction("Algorithm");

    QToolButton *settingButton = new QToolButton(this);
    settingButton->setText("Options");
    settingButton->setIcon(QIcon(":/icons/setting.png"));
    settingButton->setToolTip("Calibration setting");
    settingButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QToolButton *runButton = new QToolButton(this);
    runButton->setText("Run");
    runButton->setIcon(QIcon(":/icons/play.png"));
    runButton->setToolTip("Calibration exec");
    runButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // Add the QToolButton to a button group (if you have a function like setGroupBtn for grouping buttons)
    QHBoxLayout* ctlWidgetLayout = new QHBoxLayout(this);
    ctlWidgetLayout->addLayout(calibModelLayout);
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

void MainWindow::onAddImg1ActionTriggered() {
    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Select Sensor Data");

    // Create widgets
    QLabel *sensorLabel = new QLabel("Sensor Type:", &dialog);
    QComboBox *sensorComboBox = new QComboBox(&dialog);
    sensorComboBox->addItems({"Image Camera", "Profile Scanner"});

    QLabel *formatLabel = new QLabel("Data Format:", &dialog);
    QComboBox *formatComboBox = new QComboBox(&dialog);

    QLabel *pathLabel = new QLabel("Data Path:", &dialog);
    QLineEdit *pathLineEdit = new QLineEdit(&dialog);
    QPushButton *browseButton = new QPushButton("Browse", &dialog);

    // Function to update formatComboBox based on selected sensor type
    auto updateFormatComboBox = [&]() {
        formatComboBox->clear();
        QString sensorType = sensorComboBox->currentText();
        if (sensorType == "Image Camera") {
            formatComboBox->addItems({"JPEG", "PNG", "BMP"});
        } else if (sensorType == "Profile Scanner") {
            formatComboBox->addItems({"YML", "TXT", "JSON"});
        }
    };

    // Initialize formatComboBox
    updateFormatComboBox();

    // Connect sensorComboBox to update formatComboBox dynamically
    connect(sensorComboBox, &QComboBox::currentTextChanged, updateFormatComboBox);

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
    sensorLayout->addWidget(sensorComboBox);

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
        QString sensorType = sensorComboBox->currentText();
        QString dataFormat = formatComboBox->currentText();
        QString folderPath = pathLineEdit->text();

        if (folderPath.isEmpty() || dataFormat.isEmpty()) {
            QMessageBox::warning(this, "Error", "Data path cannot be empty.");
            return;
        } 
        else {
            try {
                // Attempt to parse the profile files and get the points
                pointsSetBuffer_ = parseProfileFiles(folderPath, dataFormat);

                // Attempt to plot the points (the first set of points from the buffer)
                viewerWin_->plotPoints(pointsSetBuffer_[0], false);

                browserWin_->setContentFromPoints(pointsSetBuffer_);

                // Connect the itemSelected signal from DockWidgetBrowser to a lambda function
                // that logs the selected dataset item's index and pose data to the log window.
                connect(browserWin_,&DockWidgetBrowser::itemSelected, [this](int index, const QString& text) {
                    viewerWin_->plotPoints(pointsSetBuffer_[index], false);
                    logWin_->log(QString("Dataset item selected - Index: %1, Pose: %2").arg(index).arg(text));
                });

                // If everything succeeds, log a success message
                logWin_->log("Dataset Loaded Successfully.");
            } catch (const std::exception& e) {
                // If any exception is thrown, log the failure message
                logWin_->log(QString("Load Dataset Failed: %1").arg(e.what()));
            } catch (...) {
                // Catch any other unexpected exceptions
                logWin_->log("Load Dataset Failed: Unknown error occurred.");
            }
        }

        // Display the selected options
        QString summary = QString("Sensor Type: %1\nData Format: %2\nData Folder: %3")
                              .arg(sensorType)
                              .arg(dataFormat)
                              .arg(folderPath);
        QMessageBox::information(this, "Selection Complete", summary);
    }
}

void MainWindow::onAddImg2ActionTriggered() {
    // Handle the logic for "Local Robot Data"
    QMessageBox::information(this, "Action Triggered", "Receiving Data From Sensor...");
    // Add logic for loading local robot data here
}

void MainWindow::onAddRob1ActionTriggered() {
    // Handle the logic for "Local Robot Data"
    QMessageBox::information(this, "Action Triggered", "Loading Local Robot Data...");
    // Add logic for loading local robot data here
}

void MainWindow::onAddRob2ActionTriggered() {
    // Handle the logic for "Receive From Robot"
    QMessageBox::information(this, "Action Triggered", "Receiving Data From Robot...");
    // Add logic for receiving data from the robot here
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

// Function to extract a numeric value from a string (filename)
double MainWindow::extractNumericValueFromFilename(const QString& filename) {
    // Regular expression to match numbers in the filename
    std::regex re(R"(\d+(\.\d+)?)");
    std::smatch match;
    
    // Convert QString to std::string for regex matching
    std::string filenameStr = filename.toStdString();
    
    // Search for the first numeric match
    if (std::regex_search(filenameStr, match, re)) {
        // Return the numeric value as a double
        return std::stod(match.str(0));
    }
    // Return 0 if no number is found
    return 0.0;
}

// Function to parse YML files from a given folder path and extract points (X, Z)
std::vector<std::vector<std::pair<double, double>>> MainWindow::parseProfileFiles(const QString& folderPath, const QString& type) {
    std::vector<std::vector<std::pair<double, double>>> pointsList;
    std::vector<std::pair<double, double>> points;

    // Convert folderPath to std::string for file handling
    std::string folderPathStr = folderPath.toStdString();

    // Use Qt's QDir to get the list of files in the directory
    QDir dir(QString::fromStdString(folderPathStr));
    dir.setFilter(QDir::Files);

    dir.setNameFilters(QStringList() << "*." + type.toLower());     // Only include files matching the type (e.g., *.yml)

    // Sort the fileInfoList by numeric value extracted from the filenames
    QFileInfoList fileInfoList = dir.entryInfoList();
    std::sort(fileInfoList.begin(), fileInfoList.end(), [=](const QFileInfo &a, const QFileInfo &b) {
        // Extract numeric values from the filenames and compare them
        double numA = extractNumericValueFromFilename(a.fileName());
        double numB = extractNumericValueFromFilename(b.fileName());
        return numA < numB;
    });
    
    // Loop through each file in the sorted list
    for (const QFileInfo& fileInfo : fileInfoList) {
        // TODO: Add other format file parse
        // Open the YAML file using OpenCV's cv::FileStorage
        cv::FileStorage fs(fileInfo.absoluteFilePath().toStdString(), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cerr << "Failed to open file: " << fileInfo.absoluteFilePath().toStdString() << std::endl;
            continue;
        }

        // Access the 'scan_line' node in the YAML file
        cv::FileNode scanLineNode = fs["scan_line"];
        if (scanLineNode.empty()) {
            std::cerr << "No 'scan_line' node found in file: " << fileInfo.absoluteFilePath().toStdString() << std::endl;
            continue;
        }

        // Loop through the 'scan_line' array and extract X and Z coordinates (Y is always 0, so it's ignored)
        for (int i = 0; i < scanLineNode.size(); i += 3) {
            double x = (double)scanLineNode[i];      // X coordinate
            double z = (double)scanLineNode[i + 2];  // Z coordinate (Y is always 0, so we skip it)
            points.push_back({x, z});  // Add point to the vector
        }
        pointsList.push_back(points);
        points.clear();

        fs.release();  // Close the file storage
    }

    return pointsList;
}

