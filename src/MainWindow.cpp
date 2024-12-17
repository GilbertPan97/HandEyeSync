#include "MainWindow.h"

#include "SAFramelessHelper.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include "SARibbonTabBar.h"
#include "SARibbonMenu.h"
#include "SARibbonComboBox.h"
#include "SARibbonLineEdit.h"
#include "SARibbonGallery.h"
#include "SARibbonCheckBox.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonApplicationButton.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonElementManager.h"
#include "SARibbonCustomizeDialog.h"

#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QTextEdit>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent):   
    SARibbonMainWindow(parent),
    customizeWidget_(nullptr)
{
    // Load the external QSS file for dark theme (use style.qss)
    QFile file(":styles/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString style = stream.readAll();
        setStyleSheet(style);
    }

    // Get screen size
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize scrnSize = screen->size();

    // Create helper widget
    SAFramelessHelper* helper = framelessHelper();
    helper->setRubberBandOnResize(false);
    setWindowTitle(("HandEyeSync"));
    setStatusBar(new QStatusBar());

    // Construct ribbon bar
    SARibbonBar* ribbon = ribbonBar();
    ribbon->setContentsMargins(5, 0, 5, 0);
    ribbon->applicationButton()->setText(("HandEye-Sync"));
    

    // Create page for profile scanner HEC
    SARibbonCategory* page_pshec = new SARibbonCategory();
    page_pshec->setCategoryName(tr("PS-HEC"));
    page_pshec->setObjectName("PS-HEC");
    ribbon->addCategoryPage(page_pshec);

    // Add quickAccessBar and rightBar
    // SARibbonQuickAccessBar* quickAccessBar = ribbon->quickAccessBar();
    // createQuickAccessBar(quickAccessBar);
    // SARibbonButtonGroupWidget* rightBar = ribbon->rightButtonGroup();
    // createRightButtonGroup(rightBar);
    // addSomeOtherAction();

    dockManager_ = new ads::CDockManager(this);
    setCentralWidget(dockManager_);
}


MainWindow::~MainWindow()
{
    // Cleanup is automatically handled by Qt, no need to manually delete the toolbars and textEdit.
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
