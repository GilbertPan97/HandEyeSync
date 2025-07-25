#ifndef DOCK_WIDGET_BROWSER_H
#define DOCK_WIDGET_BROWSER_H

#include "DockWidget.h"

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPixmap>
#include <QVector>
#include <QPair>

#include <opencv2/opencv.hpp>

/**
 * @class DockWidgetBrowser
 * @brief A customized dock widget for displaying a list of items with images and text, inheriting from ads::CDockWidget.
 */
class DockWidgetBrowser : public ads::CDockWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for DockWidgetBrowser.
     * @param title The title of the dock widget.
     * @param parent The parent widget (optional).
     */
    explicit DockWidgetBrowser(const QString& title, QWidget* parent = nullptr);

    /**
     * @brief Sets the content of the browser widget.
     * @param contentItems A vector of pairs, where each pair contains text and a corresponding image.
     */
    void setContent(const QVector<QPair<QString, QPixmap>>& contentItems);

    /**
     * @brief Clears all the content in the browser widget.
     */
    void clearContent();

    void setContentFromPoints(const std::vector<std::vector<std::pair<double, double>>>& pointsSetBuffer);

    void setContentFromRobot(const std::vector<std::vector<double>>& robDataBuffer);

private:
    QListWidget* listWidget_;       ///< A list widget to hold and display items with images and text.
    QVector<QPair<QString, QPixmap>> contentItems_;

signals:
    /**
     * @brief Emitted when an item in the listWidget is selected.
     * 
     * The signal provides the index of the selected item and its associated text.
     * 
     * @param index The index of the selected item in the list.
     * @param text The text content of the selected item.
     */
    void itemSelected(int index, const QString& text);
};

#endif // DOCK_WIDGET_BROWSER_H
