#ifndef DOCK_WIDGET_VIEWER_H
#define DOCK_WIDGET_VIEWER_H

#include "DockWidget.h"
#include "qcustomplot.h"
#include "DockWidgetProperty.h"

#include <QString>
#include <QList>
#include <QPushButton>
#include <QMouseEvent>

/**
 * @class DockWidgetViewer
 * @brief A customized dock widget inheriting from CDockWidget.
 * 
 * This class provides a custom widget for plotting data points on a QCustomPlot object.
 * The plot can either connect the points with lines or display them as discrete points, depending on the specified mode.
 */

using RenderData = std::vector<std::pair<double, double>>;

class DockWidgetViewer : public ads::CDockWidget {
    Q_OBJECT

private:
    QList<QPushButton*> buttonList_;
    QCustomPlot *customPlot_;
    RenderData *curPlotData_;
    ProfileSheet *curProfileSheet_;

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
    ~DockWidgetViewer();

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
    void plotPoints(const RenderData& points, bool connectPoints, const cv::Point3f& fea_point, int index);

    /**
     * @brief Handles resize events for the DockWidgetViewer.
     * 
     * This function is called automatically when the dock widget is resized. It ensures
     * that the QCustomPlot maintains a consistent aspect ratio (e.g., 1:1) during resizing. 
     * The function also re-renders the plot to reflect any changes.
     * 
     * @param event A pointer to the QResizeEvent object that contains information about the resize event.
     */
    void resizeEvent(QResizeEvent *event);

    /**
     * @brief Adjusts the QCustomPlot to maintain a fixed aspect ratio during resizing.
     * 
     * This function ensures that the x and y axes of the plot maintain a consistent aspect ratio 
     * (e.g., 1:1) by dynamically adjusting their ranges based on the plot area dimensions. 
     * It is typically called within the `resizeEvent` function to adapt to window size changes.
     * 
     * @param customPlot A pointer to the QCustomPlot object that needs to maintain the aspect ratio.
     */
    void keepDisplayAspectRatio(QCustomPlot *customPlot);

signals:
    void mouseClicked(const QPoint& pos);

public slots:
    // Slot to handle the pickFeatureStatus signal
    void onPickFeatureStatusChanged(bool isPicked);

// protected:
    /**
     * @brief Handles mouse press events on the plot.
     * 
     * This function is triggered when the user clicks on the plot area. It captures
     * the mouse click position and updates graph(1) with the corresponding coordinates.
     * 
     * @param event The mouse event that contains the click position.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse move events on the plot.
     * 
     * This function is triggered when the user drags the mouse on the plot area.
     * It continuously updates the point in graph(1) based on the mouse position.
     * 
     * @param event The mouse event that contains the mouse position.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    bool isEmpty(const std::pair<double, double>& fea_point);
    bool isValid(const cv::Point3f& fea_point);
    ProfileSheet parseProfileToProfileSheet(const RenderData& profile, cv::Point3f feature, int index);
    bool isMouseInsidePlot(const QPoint& pos) const;
    void updateGraph1Point(double x, double y);
};

#endif // DOCK_WIDGET_VIEWER_H
