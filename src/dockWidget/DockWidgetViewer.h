#ifndef DOCK_WIDGET_VIEWER_H
#define DOCK_WIDGET_VIEWER_H

#include "DockWidget.h"
#include "qcustomplot.h"

#include <QString>

/**
 * @class DockWidgetViewer
 * @brief A customized dock widget inheriting from CDockWidget.
 */
class DockWidgetViewer : public ads::CDockWidget {
private:
    QCustomPlot *customPlot_;

public:
    /**
     * @brief Constructor for DockWidgetViewer.
     * @param title The title of the dock widget.
     * @param parent The parent widget (optional).
     */
    explicit DockWidgetViewer(const QString& title, QWidget* parent = nullptr);

    /**
     * @brief A custom function for additional functionality.
     */
    void customFunction();

    // Add more methods as needed for your specific use case
};

#endif // DOCK_WIDGET_VIEWER_H
