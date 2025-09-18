#ifndef DOCKWIDGETVIEWER3D_H
#define DOCKWIDGETVIEWER3D_H
#include "DockWidget.h"
#include "QVtkViewer.h"

#include <QWidget>
#include <QVBoxLayout>

/**
 * @class DockWidgetViewer3D
 * @brief Dock widget encapsulating a QVtkViewer for 3D visualization.
 */
class DockWidgetViewer3D : public ads::CDockWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param title Dock widget title
     * @param parent Parent widget
     */
    explicit DockWidgetViewer3D(const QString& title,
                                QWidget* parent = nullptr);

    ~DockWidgetViewer3D() override;

    /**
     * @brief Get pointer to QVtkViewer
     * @return Pointer to QVtkViewer
     */
    QVtkViewer* viewer() const;

private:
    QVtkViewer* m_viewer;
};

#endif // DOCKWIDGETVIEWER3D_H
