#include "DockWidgetBrowser.h"

DockWidgetBrowser::DockWidgetBrowser(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent) {
    // Initialize the content label
    contentLabel = new QLabel(this);
    contentLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft); // Align content to the top-left
    contentLabel->setWordWrap(true); // Enable word wrapping
    this->setWidget(contentLabel); // Set the QLabel as the widget for the dock
}

void DockWidgetBrowser::setContent(const QString& content) {
    if (contentLabel) {
        contentLabel->setText(content);
    }
}

void DockWidgetBrowser::clearContent() {
    if (contentLabel) {
        contentLabel->clear(); // Clear the label's text
    }
}
