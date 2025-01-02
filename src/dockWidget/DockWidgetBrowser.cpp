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
}

void DockWidgetBrowser::setContent(const QVector<QPair<QString, QPixmap>>& contentItems) {
    listWidget->clear();

    for (const auto& item : contentItems) {
        QWidget* itemWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(itemWidget);

        QLabel* imageLabel = new QLabel(itemWidget);
        imageLabel->setPixmap(item.second);
        imageLabel->setFixedSize(100, 100);
        layout->addWidget(imageLabel);

        QLabel* textLabel = new QLabel(item.first, itemWidget);
        textLabel->setWordWrap(true);
        layout->addWidget(textLabel);

        QListWidgetItem* listItem = new QListWidgetItem(listWidget);
        listItem->setSizeHint(itemWidget->sizeHint());
        listWidget->addItem(listItem);
        listWidget->setItemWidget(listItem, itemWidget);
    }
}

void DockWidgetBrowser::clearContent() {
    listWidget->clear();
}
