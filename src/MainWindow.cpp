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
    DockWidgetViewer* viewerWin = new DockWidgetViewer("Viewer", this);
    DockWidgetLogger* logWin = new DockWidgetLogger("Logger", this);
    DockWidgetBrowser* browserWin = new DockWidgetBrowser("Data Browser", this);
    DockWidgetProperty* propertyWin = new DockWidgetProperty("Property Browser", this);

    dockManager_->addDockWidget(ads::TopDockWidgetArea, viewerWin);
    auto rightDockWidgetArea = dockManager_->addDockWidget(ads::RightDockWidgetArea, propertyWin);
    auto bottomDockWidgetArea = dockManager_->addDockWidget(ads::BottomDockWidgetArea, logWin);
    auto leftDockWidgetArea = dockManager_->addDockWidget(ads::LeftDockWidgetArea, browserWin);

    int width = this->width();
    int height = this->height();
    dockManager_->setSplitterSizes(bottomDockWidgetArea, {static_cast<int>(height * 0.75), static_cast<int>(height * 0.25)});
    dockManager_->setSplitterSizes(leftDockWidgetArea, {static_cast<int>(width * 0.15), static_cast<int>(width * 0.85)});
    dockManager_->setSplitterSizes(rightDockWidgetArea, {static_cast<int>(width * 0.85 * 0.7), static_cast<int>(width * 0.85 * 0.3)});
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

    QToolButton *addImgButton = new QToolButton();
    addImgButton->setIcon(QIcon(":/icons/add-images.png"));
    addImgButton->setText("Add Images");
    addImgButton->setMenu(addImgMenu);
    addImgButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addImgButton->setPopupMode(QToolButton::MenuButtonPopup);

    QMenu *addRobMenu = new QMenu(this);
    QAction *addRob1Action = addRobMenu->addAction("Local Robot Data");
    QAction *addRob2Action = addRobMenu->addAction("Receive From Robot");

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

