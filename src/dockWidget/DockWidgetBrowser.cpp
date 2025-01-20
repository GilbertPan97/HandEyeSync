#include "DockWidgetBrowser.h"

#include <QListWidgetItem>
#include <QHBoxLayout>

DockWidgetBrowser::DockWidgetBrowser(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent), listWidget(nullptr) {
    // Initialize the list widget with a vertical scrollbar
    listWidget = new QListWidget(this);

    // Ensure the list widget has a vertical scrollbar
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scrollbar
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);   // Vertical scrollbar appears as needed

    // Set the list widget as the dock's main widget
    this->setWidget(listWidget);

    // Connect the itemClicked signal to emit the custom signal directly
    connect(listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        int index = listWidget->row(item);  // Get the index of the clicked item
        QString text = item->text();       // Get the text of the clicked item
        emit itemSelected(index, text);    // Emit the custom signal
    });
}

void DockWidgetBrowser::setContent(const QVector<QPair<QString, QPixmap>>& contentItems) {
    listWidget->clear();

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
        QListWidgetItem* listItem = new QListWidgetItem(listWidget);
        listItem->setSizeHint(itemWidget->sizeHint());  // Ensure the item can resize to fit the content
        listWidget->addItem(listItem);
        listWidget->setItemWidget(listItem, itemWidget);
    }
}



void DockWidgetBrowser::clearContent() {
    listWidget->clear();
}

void DockWidgetBrowser::setContentFromPoints(const std::vector<std::vector<std::pair<double, double>>>& pointsSetBuffer) {
    // Create a vector to hold content items (image and label)
    QVector<QPair<QString, QPixmap>> contentItems;

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
        contentItems.push_back(qMakePair(QString("NoPoseData"), pixmap));
    }

    // Call the original setContent method to display the images in the widget
    setContent(contentItems);
}

