#ifndef DOCK_WIDGET_BROWSER_H
#define DOCK_WIDGET_BROWSER_H

#include "DockWidget.h" // Include the correct header for ads::CDockWidget
#include <QString>
#include <QLabel> // Used to display content

/**
 * @class DockWidgetBrowser
 * @brief A customized dock widget for displaying browser-like content, inheriting from ads::CDockWidget.
 */
class DockWidgetBrowser : public ads::CDockWidget {
public:
    /**
     * @brief Constructor for DockWidgetBrowser.
     * @param title The title of the dock widget.
     * @param parent The parent widget (optional).
     */
    explicit DockWidgetBrowser(const QString& title, QWidget* parent = nullptr);

    /**
     * @brief Sets the content to be displayed in the browser widget.
     * @param content The content to display (can be plain text or HTML).
     */
    void setContent(const QString& content);

    /**
     * @brief Clears the browser content.
     */
    void clearContent();

private:
    QLabel* contentLabel; ///< A label to display the content
};

#endif // DOCK_WIDGET_BROWSER_H
