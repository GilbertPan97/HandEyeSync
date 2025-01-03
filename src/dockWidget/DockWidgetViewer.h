#ifndef DOCK_WIDGET_VIEWER_H
#define DOCK_WIDGET_VIEWER_H

#include "DockWidget.h"
#include "qcustomplot.h"

#include <QString>

/**
 * @class DockWidgetViewer
 * @brief A customized dock widget inheriting from CDockWidget.
 * 
 * This class provides a custom widget for plotting data points on a QCustomPlot object.
 * The plot can either connect the points with lines or display them as discrete points, depending on the specified mode.
 */
class DockWidgetViewer : public ads::CDockWidget {
private:
    QCustomPlot *customPlot_; ///< Pointer to the QCustomPlot object for plotting.

public:
    /**
     * @brief Constructor for DockWidgetViewer.
     * 
     * Initializes the custom dock widget with a title and optional parent widget. 
     * It also initializes the QCustomPlot object used for displaying the data.
     * 
     * @param title The title of the dock widget.
     * @param parent The parent widget (optional). Default is nullptr.
     */
    explicit DockWidgetViewer(const QString& title, QWidget* parent = nullptr);

    /**
     * @brief Plots a set of points on the QCustomPlot object.
     * 
     * This function adds a series of points to the plot. Depending on the value of 
     * `connectPoints`, the points are either connected by lines or displayed as individual points.
     * 
     * @param points A vector of pairs, where each pair contains an x and z coordinate for a point.
     *               Example: {{1.0, 2.0}, {2.0, 3.0}, {3.0, 4.0}}.
     * @param connectPoints A boolean flag that determines whether the points should be connected by lines.
     *                      If true, the points will be connected with lines. If false, the points will
     *                      be displayed as discrete points without any connecting lines.
     */
    void plotPoints(const std::vector<std::pair<double, double>>& points, bool connectPoints);

    // Add more methods as needed for your specific use case
};

#endif // DOCK_WIDGET_VIEWER_H
