#include "DockWidgetViewer.h"

DockWidgetViewer::DockWidgetViewer(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent) {
    customPlot_ = new QCustomPlot(this);
    customPlot_->setBackground(QColor("#444444"));

    customPlot_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                 QCP::iSelectLegend | QCP::iSelectPlottables);

    // Set axis labels
    QColor penColor(200, 200, 200);
    QPen axisPen(QColor(200, 200, 200), 2);     // Create a pen with a width of 2
    customPlot_->xAxis->setLabel("X-axis");
    customPlot_->yAxis->setLabel("Z-axis");
    QFont font("Arial", 10, 5);
    customPlot_->xAxis->setLabelFont(font);
    customPlot_->yAxis->setLabelFont(font);
    customPlot_->xAxis->setLabelColor(penColor);
    customPlot_->yAxis->setLabelColor(penColor);

    // Enable grid
    customPlot_->xAxis->grid()->setVisible(true);
    customPlot_->yAxis->grid()->setVisible(true);

    // Show all axes
    customPlot_->xAxis->setVisible(true);
    customPlot_->yAxis->setVisible(true);
    customPlot_->xAxis2->setVisible(true); // Keep top x-axis visible
    customPlot_->yAxis2->setVisible(true); // Keep right y-axis visible

    // Hide ticks and labels for the top and right axes
    customPlot_->xAxis->setTickLabelColor(penColor);
    customPlot_->yAxis->setTickLabelColor(penColor);
    customPlot_->xAxis2->setTickLabels(false); // Hide tick labels for top x-axis
    customPlot_->yAxis2->setTickLabels(false); // Hide tick labels for right y-axis

    // Set pen for the axes to be bold
    customPlot_->xAxis->setBasePen(axisPen);
    customPlot_->yAxis->setBasePen(axisPen);
    customPlot_->xAxis2->setBasePen(axisPen);
    customPlot_->yAxis2->setBasePen(axisPen);

    // Set tick pen for the visible axes
    customPlot_->xAxis->setTickPen(axisPen);
    customPlot_->yAxis->setTickPen(axisPen);
    customPlot_->xAxis2->setTickPen(axisPen); // Set tick pen for top x-axis
    customPlot_->yAxis2->setTickPen(axisPen); // Set tick pen for right y-axis

    // Add a graph to the custom plot
    customPlot_->addGraph();
    customPlot_->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 8)); // Use disc markers
    customPlot_->graph(0)->setPen(QPen(QColor("#B29F25"), 2)); // Set line color and width

    // Disable lines by setting the line style to none
    customPlot_->graph(0)->setLineStyle(QCPGraph::lsNone);

    // Create a QWidget to hold the main layout
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0); // Remove margins for layout

    // Add button container to the main layout
    // layout->addWidget(buttonContainer, 0, Qt::AlignLeft | Qt::AlignTop); // Align buttons to top-left
    layout->addWidget(customPlot_, 1); // Add the custom plot below the buttons with stretch factor

    widget->setLayout(layout); // Set the layout to the widget
    this->setWidget(widget); // Set the widget with the layout to the dock widget
    this->show();
}

void DockWidgetViewer::plotPoints(const std::vector<std::pair<double, double>>& points, bool connectPoints) {
    // Clear old data
    customPlot_->graph(0)->data()->clear();  // Clear the current graph's data

    // Prepare vectors for x and z coordinates
    std::vector<double> keys;       // x coordinates
    std::vector<double> values;     // z coordinates

    // Populate keys and values with x and z coordinates from points
    for (const auto& point : points) {
        keys.push_back(point.first);        // Add x coordinate to keys
        values.push_back(point.second);     // Add z coordinate to values
    }

    // Add new data points to the graph
    customPlot_->graph(0)->addData(QVector<double>::fromStdVector(keys), QVector<double>::fromStdVector(values));

    // Set the line style based on the connectPoints parameter
    if (connectPoints) {
        customPlot_->graph(0)->setLineStyle(QCPGraph::lsLine);  // Connect the points with lines
    } else {
        customPlot_->graph(0)->setLineStyle(QCPGraph::lsNone);  // Only plot points without connecting lines
    }

    // Automatically fit the axes to the data range
    customPlot_->xAxis->rescale();
    customPlot_->yAxis->rescale();

    // Replot to update the view with the new data
    customPlot_->replot();
}
