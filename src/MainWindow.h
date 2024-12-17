#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();
    void createToolBar();
    void resizeDockWidgets(const QList<ads::CDockWidget*>& dockWidgets);
    
    void loadSettings();
    void saveSettings();

private slots:
    // Placeholder slots for menu actions
    void newFile();
    void openFile();
    void saveFile();

private:
    QTextEdit *textEdit;          // Central widget (text editor)
    QToolBar *topToolBar;         // Top toolbar

    ads::CDockManager* dockManager_;
    QList<ads::CDockWidget*> dockWidgets_;
};

#endif // MAINWINDOW_H
