#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

#include "QVtkViewer.h"   // 你提供的 QVtkViewer

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkInteractorStyleTrackballCamera.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // 创建主窗口
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("QVtkViewer Test");
    mainWindow.resize(800, 600);

    // 创建中心 Widget
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    mainWindow.setCentralWidget(centralWidget);

    // 创建 VTK 渲染窗口
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
        vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    // 创建 QVtkViewer 并绑定渲染窗口
    QVtkViewer* viewer = new QVtkViewer(renderWindow, centralWidget);
    layout->addWidget(viewer);

    // 创建 VTK 渲染器
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // 创建一个简单立方体 Actor
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(cubeSource->GetOutput());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // 设置颜色
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    // 添加 Actor 到渲染器
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("Wheat").GetData());

    vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
    auto iren = renderWidget->renderWindow()->GetInteractor();
    iren->SetInteractorStyle(style);
    setAxesSystem(iren);

    // 显示窗口
    mainWindow.show();

    return app.exec();
}
