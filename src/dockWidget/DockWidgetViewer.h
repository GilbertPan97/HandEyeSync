#ifndef DOCKWIDGETVIEWER_H
#define DOCKWIDGETVIEWER_H

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QWidget>

// DockWidgetViewer is a subclass of ads::CDockWidget
class DockWidgetViewer : public ads::CDockWidget {
    Q_OBJECT

public:
    // Constructor that initializes the dock widget with a parent widget
    explicit DockWidgetViewer(QWidget *parent = nullptr);

    // Destructor
    ~DockWidgetViewer();

private:
    // You can add additional member variables or functions here
};

#endif // DOCKWIDGETVIEWER_H
