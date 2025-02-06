#ifndef DOCK_WIDGET_PROPERTY_H
#define DOCK_WIDGET_PROPERTY_H

#include "DockWidget.h" // Include the correct header for ads::CDockWidget

#include <QTreeWidget>   // Used for displaying properties
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QStringList>

/**
 * @class DockWidgetProperty
 * @brief A customized dock widget for displaying and editing properties, inheriting from ads::CDockWidget.
 */
class DockWidgetProperty : public ads::CDockWidget {
public:
    /**
     * @brief Constructor for DockWidgetProperty.
     * @param title The title of the dock widget.
     * @param parent The parent widget (optional).
     */
    explicit DockWidgetProperty(const QString& title, QWidget* parent = nullptr);

    /**
     * @brief Initializes the property table with predefined properties.
     */
    void initPropertyTable();

    /**
     * @brief Adds a property with a simple text value.
     * @param name The name of the property.
     * @param value The value of the property.
     */
    void addProperty(const QString& name, const QString& value);

    /**
     * @brief Adds a property with a combo box for selection.
     * @param name The name of the property.
     * @param options A list of options for the combo box.
     * @param currentOption The current selected option.
     */
    void addProperty(const QString& name, const QStringList& options, const QString& currentOption);

    /**
     * @brief Adds a property with a check box for boolean values.
     * @param name The name of the property.
     * @param checked The initial checked state of the checkbox.
     */
    void addProperty(const QString& name, bool checked);

    /**
     * @brief Clears all properties from the widget.
     */
    void clearProperties();

private:
    QTreeWidget* propertyTree; ///< A tree widget to display properties
};

#endif // DOCK_WIDGET_PROPERTY_H
