
/**
 * @class QVtkViewer
 * @brief QOpenGLWidget subclass to house a vtkGenericOpenGLRenderWindow in a Qt
 * application.
 *
 * QVtkViewer extends QOpenGLWidget to make it work with a
 * QVtkViewer. it defined a 3D interactive widget.
 *
 */
#ifndef QVTKVIEWER_H
#define QVTKVIEWER_H

#include <iostream>
#include <vector>
#include <array>

#include <QOpenGLWidget>
#include <QScopedPointer>
#include <QCursor>
#include <vtkSmartPointer.h>

#include <opencv2/opencv.hpp> // for cv::Point3f

// Forward declarations for VTK classes
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkGenericOpenGLRenderWindow;
class vtkOrientationMarkerWidget;
class vtkActor;
class vtkTransformPolyDataFilter;
class vtkNamedColors;
class QVTKInteractor;
class QVTKInteractorAdapter;
class QVTKRenderWindowAdapter;

enum class zxViewerType {
    LEFT = 1,
    RIGHT,
    TOP,
    BOTTOM,
    FRONT,
    BACK,
    ISO,
};

class QVtkViewer : public QOpenGLWidget
{
    Q_OBJECT
        typedef QOpenGLWidget Superclass;

public:
    public:
    /**
     * @brief Constructor with optional parent and window flags.
     * @param parent The parent widget, default is nullptr.
     * @param f Window flags, default is no flags.
     */
    QVtkViewer(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    /**
     * @brief Constructor with an existing vtkGenericOpenGLRenderWindow.
     * @param window Pointer to a vtkGenericOpenGLRenderWindow to use.
     * @param parent The parent widget, default is nullptr.
     * @param f Window flags, default is no flags.
     */
    QVtkViewer(vtkGenericOpenGLRenderWindow* window, QWidget* parent = nullptr,
               Qt::WindowFlags f = Qt::WindowFlags());

    /** @brief Destructor, releases resources. */
    ~QVtkViewer() override;

    /**
     * @brief Display a point cloud in the viewer.
     * @param pntCloud Vector of 3D points (cv::Point3f) to display.
     * @param pntSize Size of the points in pixels.
     * @return True if the points were successfully displayed.
     */
    bool pointsDisplay(std::vector<cv::Point3f> pntCloud, int pntSize);

    /**
     * @brief Display an STL model in the viewer.
     * @param stl_path File path to the STL model.
     * @return True if the STL file was successfully loaded and displayed.
     */
    bool stlDiaplay(std::string stl_path);

    /** @brief Reset the render window (camera and scene) to default view. */
    void RenderWinReset();

    /**
     * @brief Set a vtk render window to this viewer.
     * If a render window was already set, its resources are released.
     * If the input window has no interactor, a QVTKInteractor will be created automatically.
     * @param win Pointer to the render window.
     */
    void setRenderWindow(vtkGenericOpenGLRenderWindow* win);
    void setRenderWindow(vtkRenderWindow* win);

    /**
     * @brief Get the vtkRenderWindow currently used by this viewer.
     * @return Pointer to the vtkRenderWindow.
     */
    vtkRenderWindow* GetRenderWindow();

    /**
     * @brief Get the vtkRenderWindowInteractor associated with this viewer.
     * @return Pointer to the vtkRenderWindowInteractor.
     */
    vtkRenderWindowInteractor* GetInteractor();

    /**
     * @brief Set the cursor used in the QVTK viewer.
     * @param cursor QCursor to use.
     */
    void setQVTKCursor(const QCursor& cursor);

    /**
     * @brief Returns the render window being displayed by this widget.
     * @return Pointer to the vtkRenderWindow.
     */
    vtkRenderWindow* renderWindow() const;

    /**
     * @brief Get the QVTKInteractor used by this widget.
     * @return Pointer to the QVTKInteractor.
     */
    QVTKInteractor* interactor() const;

    /**
     * @brief Get the default QSurfaceFormat for the viewer widget.
     * @param stereo_capable Enable stereo rendering if true.
     * @return QSurfaceFormat object with proper settings.
     */
    static QSurfaceFormat defaultFormat(bool stereo_capable = false);

    /**
     * @brief Enable or disable HiDPI support.
     * When enabled, the DPI is scaled according to device pixel ratio.
     * @param enable True to enable HiDPI support.
     */
    void setEnableHiDPI(bool enable);
    bool enableHiDPI() const { return this->EnableHiDPI; }

    /**
     * @brief Set or get the unscaled DPI for the render window.
     * Default is 72.
     */
    void setUnscaledDPI(int);
    int unscaledDPI() const { return this->UnscaledDPI; }

    /**
     * @brief Set or get the default cursor for the viewer.
     */
    void setDefaultCursor(const QCursor& cursor);
    const QCursor& defaultCursor() const { return this->DefaultCursor; }

    /**
     * @brief Set the main axes system (X/Y/Z) in the viewer for reference.
     * @param iren vtkRenderWindowInteractor to attach the axes to.
     */
    void setAxesSystem(vtkRenderWindowInteractor* iren);

    /**
     * @brief Set the reference axes system with optional planes and origin sphere.
     * @param showPlanes True to render XY, YZ, XZ planes, false to only render axes.
     * @param originSphereRadius Radius of a small sphere at the origin.
     */
    void setReferenceAxesSystem(bool showPlanes = false, double originSphereRadius = 0.1);

    /**
     * @brief Set the viewer type (view orientation) such as LEFT, RIGHT, TOP, ISO.
     * @param type Enum value of zxViewerType.
     */
    void setViewerType(zxViewerType type);

protected slots:
    /**
     * Called as a response to `QOpenGLContext::aboutToBeDestroyed`. This may be
     * called anytime during the widget lifecycle. We need to release any OpenGL
     * resources allocated in VTK work in this method.
     */
    virtual void cleanupContext();

    void updateSize();

protected:
    bool event(QEvent* evt) override;
    void initializeGL() override;
    void paintGL() override;

    std::vector<vtkSmartPointer<vtkActor>> MakePlanesActors(vtkNamedColors* colors);
    vtkSmartPointer<vtkTransformPolyDataFilter> MakePlane(
        std::array<int, 2>& resolution, std::array<double, 3>& origin,
        std::array<double, 3>& point1, std::array<double, 3>& point2,
        std::array<double, 4>& wxyz, std::array<double, 3>& translate);

protected:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> RenderWindow;
    QScopedPointer<QVTKRenderWindowAdapter> RenderWindowAdapter;

private:
    Q_DISABLE_COPY(QVtkViewer);

    bool EnableHiDPI;
    int UnscaledDPI;
    QCursor DefaultCursor;

    vtkSmartPointer<vtkActor> m_actor_pnts;
    
    vtkOrientationMarkerWidget* m_axesWidget;
    zxViewerType m_viewType;
};

#endif
