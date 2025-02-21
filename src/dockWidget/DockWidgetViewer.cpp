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
    widget->setStyleSheet("background: #444444;");
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    // Create a QWidget to hold the buttons
    QWidget *buttonContainer = new QWidget(widget);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(20, 10, 0, 0); // Add margins for button layout

    // Helper lambda function to create buttons
    auto createButton = [&](const QString& iconPath, const QString& toolTip,
                            const QSize& btnSize = QSize(25, 25), 
                            const QSize& iconSize = QSize(18, 18)) -> QPushButton* 
    {
        QPushButton* button = new QPushButton(QIcon(iconPath), "", this);
        button->setToolTip(toolTip);
        button->setFixedSize(btnSize);
        button->setIconSize(iconSize);
        btnList_.push_back(button);
        return button;
    };

    // Create buttons for controlling the display functionality
    QPushButton *ctlBtn_play = createButton(":/icons/play.png", "Run and pause camera grab");
    QPushButton *ctlBtn_capture = createButton(":/icons/photo-capture.png", "Capture current view");
    QPushButton *ctlBtn_download = createButton(":/icons/download0.png", "Download data");
    QPushButton *ctlBtn_upload = createButton(":/icons/upload.png", "Upload data");
    QPushButton *ctlBtn_refresh = createButton(":/icons/refresh.png", "Refresh the view");
    QPushButton *ctlBtn_trash = createButton(":/icons/trash.png", "Clear data");

    // Add buttons to the container layout
    buttonLayout->addWidget(ctlBtn_play);
    buttonLayout->addWidget(ctlBtn_capture);
    buttonLayout->addWidget(ctlBtn_download);
    buttonLayout->addWidget(ctlBtn_upload);
    buttonLayout->addWidget(ctlBtn_refresh);
    buttonLayout->addWidget(ctlBtn_trash);

    // Add button container to the main layout
    layout->addWidget(buttonContainer, 0, Qt::AlignLeft | Qt::AlignTop); // Align buttons to top-left
    layout->addWidget(customPlot_, 1); // Add the custom plot below the buttons with stretch factor

    widget->setLayout(layout); 
    this->setWidget(widget);
    this->show();
}

void DockWidgetViewer::plotPoints(const RenderData& points, bool connectPoints) {
    // Cache current plot data
    if (curPlotData_ == nullptr) {
        curPlotData_ = new RenderData();
    }
    *curPlotData_ = points; 
    
    // Clear plot old data
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
    customPlot_->rescaleAxes(true);

    // Replot to update the view with the new data
    keepDisplayAspectRatio(customPlot_);
    // customPlot_->replot();
}

void DockWidgetViewer::resizeEvent(QResizeEvent *event) {
    // Call the base class's resizeEvent to handle any parent class behavior
    // (This ensures that any default actions, such as resizing the dock widget, are executed)
    ads::CDockWidget::resizeEvent(event);

    // Check if customPlot_ is valid (make sure it has been initialized properly)
    if (customPlot_) {
        // Adjust the plot's aspect ratio to maintain a 1:1 ratio (if required)
        keepDisplayAspectRatio(customPlot_);
    }

    // Replot the graph to update the plot after resizing
    customPlot_->replot();
}

// This function dynamically adjusts the axis ranges to maintain a 1:1 aspect ratio
void DockWidgetViewer::keepDisplayAspectRatio(QCustomPlot *customPlot) {
    QCPAxisRect *axisRect = customPlot->axisRect();

    // Get the width and height of the plot area
    double width = axisRect->width();
    double height = axisRect->height();

    // Get the current axis ranges
    QCPRange xRange = customPlot->xAxis->range();
    QCPRange yRange = customPlot->yAxis->range();

    // Calculate the center points of the axes
    double xCenter = xRange.center();
    double yCenter = yRange.center();

    // Adjust the axis ranges to match the aspect ratio
    if (xRange.size() / width < yRange.size() / height) {
        // If the width is greater than the height, adjust the x-axis range
        double newXRangeSize = yRange.size() * (width / height);
        customPlot->xAxis->setRange(xCenter - newXRangeSize / 2.0, xCenter + newXRangeSize / 2.0);
    } else {
        // If the height is greater than the width, adjust the y-axis range
        double newYRangeSize = xRange.size() * (height / width);
        customPlot->yAxis->setRange(yCenter - newYRangeSize / 2.0, yCenter + newYRangeSize / 2.0);
    }

    // Replot the graph
    customPlot->replot();
}

