#ifndef DOCK_WIDGET_LOGGER_H
#define DOCK_WIDGET_LOGGER_H

#include "DockWidget.h" // Include the correct header for CDockWidget
#include <QString>
#include <QTextEdit>

/**
 * @class DockWidgetLogger
 * @brief A customized dock widget used for displaying logs, inheriting from CDockWidget.
 */
class DockWidgetLogger : public ads::CDockWidget {
    Q_OBJECT
private:
    QTextEdit* logTextEdit_;

public:
    /**
     * @brief Constructor for DockWidgetLogger.
     * @param title The title of the dock widget.
     * @param parent The parent widget (optional).
     */
    explicit DockWidgetLogger(const QString& title, QWidget* parent = nullptr);

    /**
     * @brief Logs a message to the widget.
     * @param message The message to log.
     */
    void log(const QString& message);

signals:
    void logMessageReceived(const QString &message);

private slots:
    void onLogMessageReceived(const QString &message);
};

#endif // DOCK_WIDGET_LOGGER_H
