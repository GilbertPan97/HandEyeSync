#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    // Initialize the application
    QApplication app(argc, argv);

    // Create the MainWindow instance
    MainWindow window;
    window.resize(800, 600);  // Set the window size
    window.show();  // Show the window

    // Enter the application's event loop
    return app.exec();
}
