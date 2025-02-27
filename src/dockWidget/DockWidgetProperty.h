#ifndef DOCK_WIDGET_PROPERTY_H
#define DOCK_WIDGET_PROPERTY_H

#include "DockWidget.h" // Include the correct header for ads::CDockWidget

#include <QTreeWidget>   // Used for displaying properties
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QStringList>
#include <QPushButton>

#include <opencv2/core/types.hpp>

struct ProfileSheet {
    int profileIndex;              // Profile Index
    int pointCount;                // Number of points in profile
    std::string file_path;         // Profile file path
    cv::Point3f featurePoint;      // Feature point in camera frame (for example, center of sphere or edge corner)
    bool enableFilter;             // Whether filter is enabled for this profile
    std::string filterType;        // Filter type (e.g., "Gaussian", "Median", etc.)

    ProfileSheet(int index = -1, int count = 0, const cv::Point3f& feature = cv::Point3f(0, 0, 0), bool enable = false, const std::string& filter = "")
        : profileIndex(index), pointCount(count), featurePoint(feature), enableFilter(enable), filterType(filter) {}
    // Clear method to reset the struct to default values
    void clear() {
        profileIndex = -1;
        pointCount = 0;
        file_path.clear();
        featurePoint = cv::Point3f(0, 0, 0);
        enableFilter = false;
        filterType.clear();
    }
};

/**
 * @class DockWidgetProperty
 * @brief A customized dock widget for displaying and editing properties, inheriting from ads::CDockWidget.
 */
class DockWidgetProperty : public ads::CDockWidget {
    Q_OBJECT
    
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
    void addProperty(const QString& name, const QString& value, QPushButton* button = nullptr);

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
     * @brief Writes ProfileSheet data into the properties table.
     * @param profile The ProfileSheet object containing the data to display.
     */
    void writeProfileSheetToProperties(const ProfileSheet& profile, bool button_enable);

    /**
     * @brief Clears all properties from the widget.
     */
    void clearProperties();

signals:
    void pickFeatureStatus(bool isPicked);

private:
    QTreeWidget* propertyTree; ///< A tree widget to display properties
};

#endif // DOCK_WIDGET_PROPERTY_H
