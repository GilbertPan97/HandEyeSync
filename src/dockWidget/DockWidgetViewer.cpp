#include "DockWidgetViewer.h"

DockWidgetViewer::DockWidgetViewer(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent),
      customPlot_(nullptr),
      curPlotData_(nullptr),
      curProfileSheet_(nullptr)
{
    // Initialize private member
    buttonList_ = QList<QPushButton*>();
    customPlot_ = new QCustomPlot(this);
    curPlotData_ = new RenderData();
    curProfileSheet_ = new ProfileSheet();

    // Set customPlot_
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

    // Add a graph to the custom plot: Profile 
    customPlot_->addGraph();
    customPlot_->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6));
    customPlot_->graph(0)->setPen(QPen(QColor("#B29F25"), 2));
    customPlot_->graph(0)->setLineStyle(QCPGraph::lsNone);
    
    // Add a graph to the custom plot: Feature 
    customPlot_->addGraph();
    customPlot_->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 12));
    customPlot_->graph(1)->setPen(QPen(QColor("#FF0000"), 2)); 
    customPlot_->graph(1)->setLineStyle(QCPGraph::lsNone);

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
        buttonList_.push_back(button);
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

DockWidgetViewer::~DockWidgetViewer() {
    if (curPlotData_ != nullptr) {
        delete curPlotData_;  // Free the allocated memory
    }
}

void DockWidgetViewer::plotPoints(const RenderData& points, bool connectPoints, const ProfileSheet& profile_sheet, int index) {
    // Cache current plot data
    if (curPlotData_ == nullptr) {
        curPlotData_ = new RenderData();
    }

    // Cache plot data and feature point
    *curPlotData_ = points; 
    *curProfileSheet_ = parseProfileToProfileSheet(points, profile_sheet, index);
    
    // Clear all plot old data, including graph(0) and graph(1)
    customPlot_->graph(0)->data()->clear();     // Profile graph
    customPlot_->graph(1)->data()->clear();     // Feature graph

    // Render profile data to graph(0)
    std::vector<double> keys;               // x coordinates
    std::vector<double> values;             // z coordinates
    for (const auto& point : points) {
        keys.push_back(point.first);        // Add x coordinate to keys
        values.push_back(point.second);     // Add z coordinate to values
    }
    customPlot_->graph(0)->addData(QVector<double>::fromStdVector(keys), QVector<double>::fromStdVector(values));

    // Set the line style based on the connectPoints parameter
    if (connectPoints) {
        customPlot_->graph(0)->setLineStyle(QCPGraph::lsLine);  // Connect the points with lines
    } else {
        customPlot_->graph(0)->setLineStyle(QCPGraph::lsNone);  // Only plot points without connecting lines
    }

    // Render profile data to graph(1)
    if (!isValid(profile_sheet.featurePoint)) {
        // Prepare vectors for x and z coordinates -- Profile
        std::vector<double> keys1;              // x coordinates
        std::vector<double> values1;            // z coordinates
        keys1.push_back(profile_sheet.featurePoint.x);           // Add x coordinate to keys1
        values1.push_back(profile_sheet.featurePoint.z);         // Add z coordinate to values1
        customPlot_->graph(1)->addData(QVector<double>::fromStdVector(keys1), QVector<double>::fromStdVector(values1));
    }

    // Automatically fit the axes to the data range
    customPlot_->xAxis->rescale();
    customPlot_->yAxis->rescale();
    customPlot_->rescaleAxes(true);

    // Replot to update the view with the new data
    keepDisplayAspectRatio(customPlot_);
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

void DockWidgetViewer::onFeaturePickEnable(bool enable) {
    // When the feature picking is enabled, allow updating the points
    if (enable) {
        qDebug() << "Feature picking enabled";
        // Enable mouse tracking to detect mouse movements
        setMouseTracking(true);  
        connect(customPlot_, &QCustomPlot::mousePress, this, &DockWidgetViewer::mousePressEvent);
        connect(customPlot_, &QCustomPlot::mouseMove, this, &DockWidgetViewer::mouseMoveEvent);
        // customPlot_->graph(0)->setSelectable(QCP::SelectionType::stSingleData);
    } else {
        // When feature picking is disabled, do not allow updating the points
        qDebug() << "Feature picking disabled";
        // Disable mouse tracking, so mouse events won't be detected
        setMouseTracking(false);
        disconnect(customPlot_, &QCustomPlot::mousePress, this, &DockWidgetViewer::mousePressEvent);
        disconnect(customPlot_, &QCustomPlot::mouseMove, this, &DockWidgetViewer::mouseMoveEvent);
        customPlot_->clearItems();
        customPlot_->replot(); 
        // customPlot_->graph(0)->setSelectable(QCP::SelectionType::stWhole);
    }
}

void DockWidgetViewer::mousePressEvent(QMouseEvent *event) {
    // If mouse tracking is disabled, return early and do nothing
    if (!hasMouseTracking()) return;

    // Get the mouse position and convert it to plot coordinates
    QPointF plotPos(event->pos().x(), event->pos().y());
    double x = customPlot_->xAxis->pixelToCoord(plotPos.x());
    double z = customPlot_->yAxis->pixelToCoord(plotPos.y());

    // Update the point on the graph with the new coordinates
    updateGraph1Point(x, z);
    
    qDebug() << "Mouse Pressed at: (" << x << ", " << z << ")";
}

void DockWidgetViewer::mouseMoveEvent(QMouseEvent *event) {
    // If mouse tracking is disabled, return early and do nothing
    if (!hasMouseTracking()) return;

    // Get the mouse position and convert it to plot coordinates
    QPointF plotPos(event->pos().x(), event->pos().y());
    double x = customPlot_->xAxis->pixelToCoord(plotPos.x());
    double y = customPlot_->yAxis->pixelToCoord(plotPos.y());

    // Create the crosshair lines (horizontal and vertical)
    // First clear any existing items (for dynamic updates)
    customPlot_->clearItems();

    // Draw vertical line (along the X-axis)
    QCPItemLine *verticalLine = new QCPItemLine(customPlot_);
    verticalLine->start->setCoords(x, customPlot_->yAxis->range().lower);
    verticalLine->end->setCoords(x, customPlot_->yAxis->range().upper);
    verticalLine->setPen(QPen(Qt::red)); // Use red color for vertical line

    // Draw horizontal line (along the Y-axis)
    QCPItemLine *horizontalLine = new QCPItemLine(customPlot_);
    horizontalLine->start->setCoords(customPlot_->xAxis->range().lower, y);
    horizontalLine->end->setCoords(customPlot_->xAxis->range().upper, y);
    horizontalLine->setPen(QPen(Qt::red)); // Use red color for horizontal line

    // Optional: You can also add a "+" marker at the intersection
    QCPItemText *crossText = new QCPItemText(customPlot_);
    crossText->setPositionAlignment(Qt::AlignCenter);
    crossText->setText("+");
    crossText->position->setCoords(x, y);
    crossText->setFont(QFont("Arial", 12));
    crossText->setPen(QPen(Qt::black));

    // Replot to update the display
    customPlot_->replot();
}

bool DockWidgetViewer::isEmpty(const std::pair<double, double>& fea_point) {
    // Check if both elements of the pair are NaN (representing "empty")
    return std::isnan(fea_point.first) && std::isnan(fea_point.second);
}

bool DockWidgetViewer::isValid(const cv::Point3f& fea_point) {
    return !std::isnan(fea_point.x) && !std::isnan(fea_point.y) && !std::isnan(fea_point.z) &&
           fea_point.x != 0.0f && fea_point.y != 0.0f && fea_point.z != 0.0f;
}

ProfileSheet DockWidgetViewer::parseProfileToProfileSheet(const RenderData& profile, const ProfileSheet sheet, int index) {
    // Create a ProfileSheet
    ProfileSheet profileSheet;
    profileSheet.profileIndex = index;                              // Set the profile index
    profileSheet.file_path = sheet.file_path;
    profileSheet.pointCount = static_cast<int>(profile.size());     // Set the number of points in the profile
    profileSheet.enableFilter = false;                              // Set the filter flag (can be adjusted)
    profileSheet.filterType = "";                                   // Set filter type (can be adjusted)

    profileSheet.featurePoint = sheet.featurePoint;                 // Leave featurePoint empty or set to a default value (0,0,0)

    return profileSheet;
}

bool DockWidgetViewer::isMouseInsidePlot(const QPoint& pos) const {
    // Check if the mouse position is inside the plot's bounding box
    QRect plotArea = customPlot_->axisRect()->rect();  // Use rect() instead of boundingRect()
    return plotArea.contains(pos);
}

void DockWidgetViewer::updateGraph1Point(double x, double z) {
    // Check if customPlot_ is valid
    if (customPlot_ == nullptr) {
        throw std::runtime_error("customPlot_ is null!");
    }

    // Check if graph(1) is valid
    if (customPlot_->graphCount() <= 1 || customPlot_->graph(1) == nullptr) {
        throw std::runtime_error("Graph 1 does not exist!");
    }

    // Optional: Validate x and z (you can adjust the validation based on your requirements)
    if (std::isnan(x) || std::isnan(z)) {
        throw std::runtime_error("Invalid data: x or z is NaN!");
    }

    // Replot to refresh the view in graph(1)
    customPlot_->graph(1)->data()->clear();
    customPlot_->graph(1)->addData(x, z);
    customPlot_->replot();

    // Update featurePoint if curProfileSheet_ is valid
    if (curProfileSheet_ != nullptr) {
        curProfileSheet_->featurePoint = cv::Point3f(x, 0, z);
        emit updatedFeaturePoint(*curProfileSheet_);
    } else {
        throw std::runtime_error("curProfileSheet_ is null!");
    }
}