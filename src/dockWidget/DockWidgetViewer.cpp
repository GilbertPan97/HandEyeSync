#include "DockWidgetViewer.h"

DockWidgetViewer::DockWidgetViewer(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent) {
    customPlot_ = new QCustomPlot(this);

    customPlot_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                 QCP::iSelectLegend | QCP::iSelectPlottables);

    // Set axis labels
    customPlot_->xAxis->setLabel("X-axis");
    customPlot_->yAxis->setLabel("Z-axis");

    // Enable grid
    customPlot_->xAxis->grid()->setVisible(true);
    customPlot_->yAxis->grid()->setVisible(true);

    // Show all axes
    customPlot_->xAxis->setVisible(true);
    customPlot_->yAxis->setVisible(true);
    customPlot_->xAxis2->setVisible(true); // Keep top x-axis visible
    customPlot_->yAxis2->setVisible(true); // Keep right y-axis visible

    // Hide ticks and labels for the top and right axes
    customPlot_->xAxis2->setTickLabels(false); // Hide tick labels for top x-axis
    customPlot_->yAxis2->setTickLabels(false); // Hide tick labels for right y-axis

    // Set pen for the axes to be bold
    QPen axisPen(Qt::black, 2); // Create a pen with a width of 2
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
    customPlot_->graph(0)->setPen(QPen(Qt::blue, 2)); // Set line color and width

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

void DockWidgetViewer::customFunction() {
    // Add custom logic here
}
