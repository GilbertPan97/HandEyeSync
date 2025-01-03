#include "DockWidgetProperty.h"

DockWidgetProperty::DockWidgetProperty(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent) {
    // Initialize the tree widget
    propertyTree = new QTreeWidget(this);
    propertyTree->setColumnCount(2);        // Two columns: Name and Value
    propertyTree->setHeaderLabels({"Property", "Value"});   // Set column headers

    // Apply dark color scheme using QPalette
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(35, 35, 35));  // Dark background color
    palette.setColor(QPalette::Foreground, QColor(200, 200, 200)); // Light text color
    propertyTree->setPalette(palette);
    
    // Apply a dark color scheme to the headers (optional)
    QHeaderView* header = propertyTree->header();
    header->setStyleSheet("QHeaderView::section { background-color: #4d4d4d; color: white; }");

    // Apply additional custom styling using stylesheet
    propertyTree->setStyleSheet(
        "QTreeWidget { "
        "background-color: #444444; "
        "color: rgb(200, 200, 200); "
        "selection-background-color: #575757; "
        "selection-color: white; "
        "} "
        "QTreeWidget::item:hover { "
        "background-color: #4d4d4d; "
        "} "
        "QTreeWidget::item:selected { "
        "background-color: #575757; "
        "}"
    );

    this->setWidget(propertyTree);  // Set the QTreeWidget as the widget for the dock
}


void DockWidgetProperty::addProperty(const QString& name, const QString& value) {
    if (propertyTree) {
        QTreeWidgetItem* item = new QTreeWidgetItem(propertyTree);
        item->setText(0, name);     // Set property name in the first column
        item->setText(1, value);    // Set property value in the second column
    }
}

void DockWidgetProperty::clearProperties() {
    if (propertyTree) {
        propertyTree->clear();      // Clear all items from the tree widget
    }
}
