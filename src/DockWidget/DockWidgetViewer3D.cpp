#include "DockWidgetViewer3D.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>

DockWidgetViewer3D::DockWidgetViewer3D(const QString& title,
                                       QWidget* parent)
    : ads::CDockWidget(title, parent)
{
    // Create main container widget
    QWidget* container = new QWidget(this);

    // Create vertical layout
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0); // no margin

    // Create QVtkViewer instance with a new render window
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> rw =
        vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_viewer = new QVtkViewer(rw, container);

    // Add QVtkViewer to layout
    layout->addWidget(m_viewer);

    container->setLayout(layout);
    this->setWidget(container); // Set container as dock widget content
}

DockWidgetViewer3D::~DockWidgetViewer3D()
{
    // QVtkViewer will be deleted by Qt parent-child system
}

QVtkViewer* DockWidgetViewer3D::viewer() const
{
    return m_viewer;
}
