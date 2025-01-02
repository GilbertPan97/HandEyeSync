#include "DockWidgetProperty.h"

DockWidgetProperty::DockWidgetProperty(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent) {
    // Initialize the tree widget
    propertyTree = new QTreeWidget(this);
    propertyTree->setColumnCount(2);        // Two columns: Name and Value
    propertyTree->setHeaderLabels({"Property", "Value"});   // Set column headers
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
