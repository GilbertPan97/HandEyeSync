#include "DockWidgetBrowser.h"

#include <QListWidgetItem>
#include <QHBoxLayout>

DockWidgetBrowser::DockWidgetBrowser(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent), listWidget_(nullptr) {
    // Initialize the list widget with a vertical scrollbar
    listWidget_ = new QListWidget(this);

    // Ensure the list widget has a vertical scrollbar
    listWidget_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scrollbar
    listWidget_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);   // Vertical scrollbar appears as needed

    // Set the list widget as the dock's main widget
    this->setWidget(listWidget_);

    // Connect the itemClicked signal to emit the custom signal directly
    connect(listWidget_, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        int index = listWidget_->row(item);  // Get the index of the clicked item
        QString text = contentItems_[index].first;       // Get the text of the clicked item
        emit itemSelected(index, text);    // Emit the custom signal
    });
}

void DockWidgetBrowser::setContent(const QVector<QPair<QString, QPixmap>>& contentItems) {
    listWidget_->clear();

    for (int i = 0; i < contentItems.size(); ++i) {
        const auto& item = contentItems[i];

        QWidget* itemWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(itemWidget);

        // Add a label with the index (order number) on the left
        QLabel* indexLabel = new QLabel(QString::number(i + 1), itemWidget);
        indexLabel->setAlignment(Qt::AlignCenter);

        // Set the background color to yellow (#B29F25) and fix the height to 60
        QFont font = indexLabel->font();
        font.setBold(true);
        indexLabel->setFont(font);
        indexLabel->setStyleSheet("background-color: #B29F25");
        indexLabel->setFixedSize(20, 60);

        layout->addWidget(indexLabel);

        // Add the image label
        QLabel* imageLabel = new QLabel(itemWidget);
        imageLabel->setFixedSize(60, 60);  // Ensure fixed size for image
        imageLabel->setScaledContents(true);  // Allow the image to scale within the fixed size
        imageLabel->setPixmap(item.second);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setStyleSheet("border: 3px solid #B29F25;");
        layout->addWidget(imageLabel);

        // Add the text label
        QLabel* textLabel = new QLabel(item.first, itemWidget);
        textLabel->setWordWrap(true);
        textLabel->setFont(font);
        layout->addWidget(textLabel);

        // Create the list item and set the widget
        QListWidgetItem* listItem = new QListWidgetItem(listWidget_);
        listItem->setSizeHint(itemWidget->sizeHint());  // Ensure the item can resize to fit the content
        listWidget_->addItem(listItem);
        listWidget_->setItemWidget(listItem, itemWidget);
    }
}



void DockWidgetBrowser::clearContent() {
    listWidget_->clear();
}

void DockWidgetBrowser::setContentFromPoints(const std::vector<std::vector<std::pair<double, double>>>& pointsSetBuffer) {
    // Clear all old contentItems before reset content
    contentItems_.clear();
    
    // Iterate through each set of points
    for (const auto& pointsSet : pointsSetBuffer) {
        // Calculate the bounding box (min/max) of the points set
        double minX = std::numeric_limits<double>::max(), minY = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest(), maxY = std::numeric_limits<double>::lowest();

        for (const auto& point : pointsSet) {
            minX = std::min(minX, point.first);
            minY = std::min(minY, point.second);
            maxX = std::max(maxX, point.first);
            maxY = std::max(maxY, point.second);
        }

        // Calculate the width and height of the bounding box
        double width = maxX - minX;
        double height = maxY - minY;

        // Create a blank image with black background
        cv::Mat img(500, 500, CV_8UC3, cv::Scalar(40, 40, 40)); // Black background

        // Scale factor to ensure points fit inside the image with padding
        double scale = std::min(img.cols / width, img.rows / height); // Keep aspect ratio

        // Add padding factor to leave some space around the points (e.g., 90% of available space)
        double paddingFactor = 0.9;  // 90% of available space for points
        scale *= paddingFactor;

        // Calculate the center of the point set
        double centerX = minX + width / 2.0;
        double centerY = minY + height / 2.0;

        // Define the color for points: white (BGR = 255, 255, 255)
        cv::Scalar pointColor(255, 255, 255); // White color for points in BGR format

        // Draw the points on the image, scaled and offset so the center aligns with image center
        for (const auto& point : pointsSet) {
            // Scale and translate the points to fit the image center
            int x = static_cast<int>((point.first - centerX) * scale + img.cols / 2);
            int y = static_cast<int>((point.second - centerY) * scale + img.rows / 2);
            
            // Ensure the point is within image bounds
            x = std::max(0, std::min(x, img.cols - 1));
            y = std::max(0, std::min(y, img.rows - 1));

            // Draw each point with the specified white color
            cv::circle(img, cv::Point(x, y), 5, pointColor, -1); // Draw each point as a white circle
        }

        // Convert the OpenCV Mat image to a QPixmap
        QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        QPixmap pixmap = QPixmap::fromImage(qimg);

        // Add the generated image and label ("NoPoseData") to the content list
        contentItems_.push_back(qMakePair(QString("NoPoseData"), pixmap));
    }

    // Call the original setContent method to display the images in the widget
    setContent(contentItems_);
}

void DockWidgetBrowser::setContentFromRobot(const std::vector<std::vector<double>>& robDataBuffer) {

    // Check if contentItems_ size matches robDataBuffer size
    if (contentItems_.size() != robDataBuffer.size()) {
        throw std::runtime_error("ListWidget contentItems_ size does not match robDataBuffer size!");
    }

    // Iterate through robDataBuffer and update the strings in contentItems_
    for (size_t i = 0; i < robDataBuffer.size(); ++i) {
        // Convert robDataSet to a string
        QString robDataStr;
        
        if (robDataBuffer[i].size() == 6) {
            // Define the labels
            const QString labels[] = { "x", "y", "z", "w", "p", "r" };

            // Iterate through the 6 values and add corresponding labels
            for (size_t j = 0; j < robDataBuffer[i].size(); ++j) {
                robDataStr += labels[j] + ": " + QString::number(robDataBuffer[i][j], 'f', 2) + " ";  // Add label and formatted value
            }
        } else {
            // If not 6 values, just add them as a regular string (without labels)
            robDataStr = "Wrong Data:\n";
            for (const auto& value : robDataBuffer[i]) {
                robDataStr += QString::number(value, 'f', 2) + " ";  // Format each value to 2 decimal places
            }
        }

        // Update the label (string) in contentItems_ to the formatted string
        contentItems_[i].first = robDataStr;  // Replace the "NoPoseData" with the formatted string
    }

    // Call setContent to update the UI
    setContent(contentItems_);
}

