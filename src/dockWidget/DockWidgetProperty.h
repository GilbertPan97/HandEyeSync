#ifndef DOCK_WIDGET_PROPERTY_H
#define DOCK_WIDGET_PROPERTY_H

#include "DockWidget.h" // Include the correct header for ads::CDockWidget
#include <QTreeWidget> // Used for displaying properties
#include <QString>

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
     * @brief Adds a property to the widget.
     * @param name The name of the property.
     * @param value The value of the property.
     */
    void addProperty(const QString& name, const QString& value);

    /**
     * @brief Clears all properties from the widget.
     */
    void clearProperties();

private:
    QTreeWidget* propertyTree; ///< A tree widget to display properties
};

#endif // DOCK_WIDGET_PROPERTY_H
