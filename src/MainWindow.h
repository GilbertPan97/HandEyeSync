#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QTextStream>
#include <QWidget>

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"
#include "SARibbonMainWindow.h"

class SARibbonCategory;
class SARibbonContextCategory;
class SARibbonCustomizeWidget;
class SARibbonActionsManager;
class SARibbonQuickAccessBar;
class SARibbonButtonGroupWidget;

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Placeholder slots for menu actions
    void newFile();
    void openFile();
    void saveFile();

private:
    QTextEdit *textEdit;          // Central widget (text editor)
    QToolBar *topToolBar;         // Top toolbar
    QToolBar *leftToolBar;        // Left toolbar

    SARibbonCustomizeWidget* customizeWidget_;
    ads::CDockManager* dockManager_;
};

#endif // MAINWINDOW_H
