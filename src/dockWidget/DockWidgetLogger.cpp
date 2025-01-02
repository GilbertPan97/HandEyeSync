#include "DockWidgetLogger.h"
#include <QDateTime>
#include <QTextEdit>

// Constructor
DockWidgetLogger::DockWidgetLogger(const QString& title, QWidget* parent)
    : ads::CDockWidget(title, parent) {
    // Create a QTextEdit widget for displaying logs
    logTextEdit_ = new QTextEdit(this);
    logTextEdit_->setReadOnly(true);        // Make it read-only
    logTextEdit_->setStyleSheet("QTextEdit { background-color: white; }");

    // Add the log text edit to the dock widget
    this->setWidget(logTextEdit_);

    // Initialize connection for the log message signal
    connect(this, &DockWidgetLogger::logMessageReceived, this, &DockWidgetLogger::onLogMessageReceived);
    log("Logger Initialized Successfully.");
}

// External interface to log messages
void DockWidgetLogger::log(const QString& message) {
    // Emit the signal to handle the message
    emit logMessageReceived(message);
}

// Slot to handle received log messages
void DockWidgetLogger::onLogMessageReceived(const QString &message) {
    // Get the current date and time and format it
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); // Format can be adjusted as needed

    // Prepend the current time and ">> " to the message for console-like output
    QString formattedMessage = QString("[%1] >> %2").arg(currentTime, message);
    
    // Append the formatted message to the log text edit
    logTextEdit_->append(formattedMessage);     // Display the log message in the log viewer
}
