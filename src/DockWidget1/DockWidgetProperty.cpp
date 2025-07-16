#include "DockWidgetProperty.h"

#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHeaderView>

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
    propertyTree->setIndentation(0);
    
    // Apply a dark color scheme to the headers (optional)
    QHeaderView* header = propertyTree->header();
    header->setStyleSheet("QHeaderView::section { background-color: #4d4d4d; color: white; }");

    // Apply stylesheet to ensure alignment and table borders
    propertyTree->setStyleSheet(
        "QTreeWidget { "
        "background-color: #444444; "  // Dark background
        "color: rgb(200, 200, 200); "  // Light gray text
        "selection-background-color: #686868; "  // Lighter selection background
        "selection-color: white; "  // Text color when selected
        "} "
        "QHeaderView::section { "
        "background-color: #333333; "  // Dark header background
        "color: white; "  // Header text color
        "border: 1px solid #222222; "  // Header border
        "text-align: left; "  // Left align header text
        "padding-left: 10px; "  // Add padding to align with content
        "} "
        "QTreeWidget::item { "
        "border-bottom: 1px solid #333333; "  // Bottom border for each row
        "padding: 5px; "  // Add padding for better spacing
        "} "
        "QTreeWidget::item:hover { "
        "background-color: #4d4d4d; "  // Hover effect
        "} "
        "QTreeWidget::item:selected { "
        "background-color: #686868; "  // Lighter selection effect
        "} "
    );

    this->setWidget(propertyTree);  // Set QTreeWidget as the main widget for the dock

    // Initialize the property table
    initPropertyTable();
}

// Initialize the property table with predefined attributes
void DockWidgetProperty::initPropertyTable() {
    // 1. Data Index
    addProperty("Data Index", QString(" "));

    // 2. Points
    addProperty("Points", QString(" "));

    // 3. Point Count
    addProperty("File Path", QString(" "));

    // 4. Feature
    QPushButton* pickBtn = new QPushButton();
    pickBtn->setIcon(QIcon(":/icons/picking.png"));
    pickBtn->setEnabled(false);
    addProperty("Feature Coord", QString("X: 0, Y: 0, Z: 0"), pickBtn);

    // 5. Filter Enable
    addProperty("Filter Enable", false);

    // 6. Filter Tools
    QStringList filterOptions = {"No Filter", "Gaussian", "Median"};
    addProperty("Filter Tools", filterOptions, "No Filter");
}

// Add a generic property with a text value
void DockWidgetProperty::addProperty(const QString& name, const QString& value, QPushButton* button) {
    if (propertyTree) {
        QTreeWidgetItem* item = new QTreeWidgetItem(propertyTree);
        item->setText(0, name);  // Set property name in the first column

        // Make the property name bold
        QFont font = item->font(0);
        font.setBold(true);
        font.setPointSize(font.pointSize() - 1); 
        item->setFont(0, font);

        // Create a QLineEdit to allow copying the value
        QLineEdit* lineEdit = new QLineEdit(value);
        lineEdit->setReadOnly(true);  // Set the line edit to be read-only (non-editable)

        // Create a horizontal layout to hold the QLineEdit and QPushButton (if any)
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(lineEdit);

        if (button) {
            layout->addWidget(button);  // If a button is provided, add it to the layout
        }

        layout->setContentsMargins(0, 0, 0, 0);  // Remove margins around the layout

        // Create a widget that will contain the layout
        QWidget* widget = new QWidget();
        widget->setLayout(layout);

        // Set the widget (containing both the QLineEdit and optionally the QPushButton) as the item widget
        propertyTree->setItemWidget(item, 1, widget);
    }
}

// Add a property with a combo box for selection
void DockWidgetProperty::addProperty(const QString& name, const QStringList& options, const QString& currentOption) {
    if (propertyTree) {
        QTreeWidgetItem* item = new QTreeWidgetItem(propertyTree);
        item->setText(0, name);  // Set property name in the first column

        // Make the property name bold
        QFont font = item->font(0);
        font.setBold(true);
        font.setPointSize(font.pointSize() - 1); 
        item->setFont(0, font);

        // Create a QComboBox for selecting property value
        QComboBox* comboBox = new QComboBox();
        comboBox->addItems(options);  // Add options to the combo box
        comboBox->setCurrentText(currentOption);  // Set the default selected option

        // Set the combo box as the widget in the second column
        propertyTree->setItemWidget(item, 1, comboBox);
    }
}

// Add a property with a checkbox for boolean values
void DockWidgetProperty::addProperty(const QString& name, bool checked) {
    if (propertyTree) {
        QTreeWidgetItem* item = new QTreeWidgetItem(propertyTree);
        item->setText(0, name);  // Set property name in the first column

        // Make the property name bold
        QFont font = item->font(0);
        font.setBold(true);
        font.setPointSize(font.pointSize() - 1); 
        item->setFont(0, font);

        // Create a QCheckBox for boolean property value
        QCheckBox* checkBox = new QCheckBox();
        checkBox->setChecked(checked);  // Set initial checked state

        // Set the checkbox as the widget in the second column
        propertyTree->setItemWidget(item, 1, checkBox);
    }
}

void DockWidgetProperty::writeProfileSheetToProperties(const ProfileSheet& profile, bool button_enable) {
    // Clear existing properties before adding new ones
    clearProperties();

    // 1. Profile Index
    addProperty("Profile Index", QString::number(profile.profileIndex + 1));

    // 2. Point Count
    addProperty("Point Count", QString::number(profile.pointCount));

    // 3. Point Count
    addProperty("File Path", QString::fromStdString(profile.file_path));

    // 4. Feature Point (X, Y, Z)
    QPushButton* pickBtn = new QPushButton();
    pickBtn->setIcon(QIcon(":/icons/picking.png"));
    QString featurePoint = QString("X: %1, Y: %2, Z: %3")
                               .arg(profile.featurePoint.x)
                               .arg(profile.featurePoint.y)
                               .arg(profile.featurePoint.z);
    if (button_enable) {
        pickBtn->setCheckable(true);
        emit pickFeatureStatus(false);      // Initial with false
        connect(pickBtn, &QPushButton::toggled, [this, pickBtn](bool checked) {
            if (checked) {
                pickBtn->setChecked(true);
                pickBtn->setStyleSheet("background-color: #4CAF50; color: white;");
            } 
            else {
                pickBtn->setChecked(false);
                pickBtn->setStyleSheet("background-color: #444444; color: white;");
            }
            emit pickFeatureStatus(checked);    // Emit signal for feature point picking in DockWidgetViewer
        });
        addProperty("Feature Point", featurePoint, pickBtn);
    } else 
        addProperty("Feature Point", featurePoint);

    // 5. Filter Enable
    addProperty("Filter Enable", profile.enableFilter);

    // 6. Filter Type
    QStringList filterOptions = {"No Filter", "Gaussian", "Median"};
    addProperty("Filter Tools", filterOptions, QString::fromStdString(profile.filterType));
}


// Clear all properties from the tree widget
void DockWidgetProperty::clearProperties() {
    if (propertyTree) {
        propertyTree->clear();  // Remove all items from the tree widget
    }
}
