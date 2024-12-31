#include "DockWidgetViewer.h"

#include <QWidget>

DockWidgetViewer::DockWidgetViewer(QWidget *parent)
    : ads::CDockWidget("Viewer", parent) // Initialize the base class (CDockWidget)
{
    // // Create a QWidget to serve as the content of the dock widget
    // QWidget *viewerContent = new QWidget(this); 

    // // Set the created QWidget as the content widget of the dock widget
    // setWidget(viewerContent);

    // // Set properties for the dock widget
    // setAllowedAreas(Qt::AllDockWidgetAreas);  // Allow docking in any area
    // setFloating(true);  // Allow the dock widget to float
}

DockWidgetViewer::~DockWidgetViewer() {
    // Destructor, you can add cleanup code if needed
}
