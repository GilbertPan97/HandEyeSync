#include <iostream>

#include <QVTKRenderWidget.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkPLYReader.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkProperty2D.h>
#include <vtkCaptionActor2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAutoInit.h> 
#include <vtkOrientationMarkerWidget.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkPropAssembly.h>

#include <opencv2/opencv.hpp>

#include <QLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QFileDialog>

VTK_MODULE_INIT(vtkRenderingOpenGL2);   // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);

void setAxesSystem(vtkRenderWindowInteractor* iren)
{
    if (!iren) return;

    vtkOrientationMarkerWidget* axesWidget = vtkOrientationMarkerWidget::New();

    vtkSmartPointer<vtkAnnotatedCubeActor> cube = vtkSmartPointer<vtkAnnotatedCubeActor>::New();
    cube->SetFaceTextScale(0.65);
    cube->GetCubeProperty()->SetColor(0.9, 0.9, 0.9);
    cube->GetTextEdgesProperty()->SetLineWidth(1);
    cube->GetTextEdgesProperty()->SetDiffuse(0);
    cube->GetTextEdgesProperty()->SetAmbient(1);
    cube->GetTextEdgesProperty()->SetColor(0.24, 0.24, 0.24);
    vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();

    // Set cube face text
    cube->SetXPlusFaceText("L");  
    cube->SetXMinusFaceText("R");
    cube->SetYPlusFaceText("P");  
    cube->SetYMinusFaceText("A");
    cube->SetZPlusFaceText("S");  
    cube->SetZMinusFaceText("I");

    // Color each cube face
    cube->GetXPlusFaceProperty()->SetColor(1,0,0); cube->GetXPlusFaceProperty()->SetInterpolationToFlat();
    cube->GetXMinusFaceProperty()->SetColor(1,0,0); cube->GetXMinusFaceProperty()->SetInterpolationToFlat();
    cube->GetYPlusFaceProperty()->SetColor(0,1,0); cube->GetYPlusFaceProperty()->SetInterpolationToFlat();
    cube->GetYMinusFaceProperty()->SetColor(0,1,0); cube->GetYMinusFaceProperty()->SetInterpolationToFlat();
    cube->GetZPlusFaceProperty()->SetColor(0,0,1); cube->GetZPlusFaceProperty()->SetInterpolationToFlat();
    cube->GetZMinusFaceProperty()->SetColor(0,0,1); cube->GetZMinusFaceProperty()->SetInterpolationToFlat();

    // Create XYZ axes actor
    vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
    axesActor->SetShaftTypeToCylinder();
    axesActor->SetXAxisLabelText("x");
    axesActor->SetYAxisLabelText("y");
    axesActor->SetZAxisLabelText("z");
    axesActor->GetXAxisShaftProperty()->SetColor(1,0,0);
    axesActor->GetXAxisTipProperty()->SetColor(1,0,0);
    axesActor->GetYAxisShaftProperty()->SetColor(0,1,0);
    axesActor->GetYAxisTipProperty()->SetColor(0,1,0);
    axesActor->GetZAxisShaftProperty()->SetColor(0,0,1);
    axesActor->GetZAxisTipProperty()->SetColor(0,0,1);
    axesActor->SetAxisLabels(1);
    axesActor->SetTotalLength(1.5,1.5,1.5);

    vtkTextProperty* tprop = axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
    tprop->ItalicOn(); tprop->ShadowOn(); tprop->SetFontFamilyToTimes();
    axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);
    axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);

    vtkSmartPointer<vtkPropAssembly> assembly = vtkSmartPointer<vtkPropAssembly>::New();
    assembly->AddPart(axesActor);
    assembly->AddPart(cube);

    axesWidget->SetOutlineColor(1,1,1);
    axesWidget->SetViewport(0.0,0.0,0.2,0.2);
    axesWidget->SetOrientationMarker(assembly);
    axesWidget->SetInteractor(iren);
    axesWidget->On();
    axesWidget->SetInteractive(0);
}

// ------------------------
// Create a grid ground on XY plane
// ------------------------
// sizeX: total length in X direction
// sizeY: total length in Y direction
// divisionsX: number of divisions in X direction
// divisionsY: number of divisions in Y direction
vtkSmartPointer<vtkActor> createGridGround(double sizeX=10.0, double sizeY=10.0, int divisionsX=10, int divisionsY=10)
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

    double stepX = sizeX / divisionsX;
    double stepY = sizeY / divisionsY;
    double startX = -sizeX / 2.0;
    double startY = -sizeY / 2.0;

    // Lines parallel to X axis
    for(int i=0; i<=divisionsY; ++i){
        double y = startY + i*stepY;
        vtkIdType ids[2] = { points->InsertNextPoint(startX, y, 0), points->InsertNextPoint(startX + sizeX, y, 0) };
        lines->InsertNextCell(2, ids);
    }

    // Lines parallel to Y axis
    for(int i=0; i<=divisionsX; ++i){
        double x = startX + i*stepX;
        vtkIdType ids[2] = { points->InsertNextPoint(x, startY, 0), points->InsertNextPoint(x, startY + sizeY, 0) };
        lines->InsertNextCell(2, ids);
    }

    vtkSmartPointer<vtkPolyData> gridPolyData = vtkSmartPointer<vtkPolyData>::New();
    gridPolyData->SetPoints(points);
    gridPolyData->SetLines(lines);

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(gridPolyData);

    vtkSmartPointer<vtkActor> gridActor = vtkSmartPointer<vtkActor>::New();
    gridActor->SetMapper(mapper);
    gridActor->GetProperty()->SetColor(0.5, 0.5, 0.5); // gray color
    gridActor->GetProperty()->SetLineWidth(1);

    return gridActor;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ------------------------
    // Create main QWidget and layouts
    // ------------------------
    QWidget* w = new QWidget();
    QHBoxLayout* h_layout = new QHBoxLayout(); // Horizontal layout for buttons
    QVBoxLayout* v_layout = new QVBoxLayout(w); // Vertical layout to hold buttons and render widget

    // ------------------------
    // Create VTK render widget and control buttons
    // ------------------------
    QVTKRenderWidget* renderWidget = new QVTKRenderWidget(); // Widget to display VTK render window
    QPushButton* loadPly_btn = new QPushButton("Load PLY PointCloud", w); // Button to load PLY file
    QPushButton* loadStl_btn = new QPushButton("Load STL Model", w);      // Button to load STL file
    QPushButton* show_btn = new QPushButton("Display", w);                 // Button to show the loaded object
    QPushButton* clear_btn = new QPushButton("Clear", w);                  // Button to clear the render scene

    // Add buttons to horizontal layout
    h_layout->addWidget(loadPly_btn);
    h_layout->addWidget(loadStl_btn);

    // Add layouts and render widget to vertical layout
    v_layout->addLayout(h_layout);
    v_layout->addWidget(show_btn);
    v_layout->addWidget(clear_btn);
    v_layout->addWidget(renderWidget);

    // ------------------------
    // Create VTK objects
    // ------------------------
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New(); // VTK renderer
    renderer->SetBackground(0.1, 0.2, 0.4); // Set background color
    renderWidget->renderWindow()->AddRenderer(renderer); // Add renderer to QVTKRenderWidget

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New(); // Actor to represent geometry
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New(); // Mapper for polydata
    
    // ------------------------
    // Add grid ground to renderer
    // ------------------------
    vtkSmartPointer<vtkActor> grid = createGridGround(10.0, 10.0, 10, 10);
    renderer->AddActor(grid);

    // ------------------------
    // Connect Load PLY button (currently commented out)
    // ------------------------
    // NOTE: This block is commented because PLY loading is not yet implemented.
    // Once implemented, vtkPLYReader will read the point cloud and mapper/actor will display it.
    QObject::connect(loadPly_btn, &QPushButton::released, [&](){
        QString plyFilePath = QFileDialog::getOpenFileName(w, "Open PLY PointCloud", "", "PLY Files (*.ply)");
        if (!plyFilePath.isEmpty()) {
            vtkSmartPointer<vtkPLYReader> plyReader = vtkSmartPointer<vtkPLYReader>::New();
            plyReader->SetFileName(plyFilePath.toStdString().c_str());
            plyReader->Update();
        
            vtkSmartPointer<vtkPolyData> plyData = plyReader->GetOutput();

            // print number of points
            std::cout << "Number of points in PLY:" << plyData->GetNumberOfPoints();

            if (plyData->GetNumberOfPoints() > 0 && plyData->GetNumberOfVerts() == 0) {
                vtkSmartPointer<vtkCellArray> verts = vtkSmartPointer<vtkCellArray>::New();
                for (vtkIdType i = 0; i < plyData->GetNumberOfPoints(); i++) {
                    verts->InsertNextCell(1);
                    verts->InsertCellPoint(i);
                }
                plyData->SetVerts(verts);
            }

            mapper->SetInputData(plyData);
            actor->SetMapper(mapper);
            actor->GetProperty()->SetColor(51/255.0, 63/255.0, 80/255.0);
            actor->GetProperty()->SetPointSize(5);
        }
        show_btn->clicked();
    });

    // ------------------------
    // Connect Load STL button
    // ------------------------
    QObject::connect(loadStl_btn, &QPushButton::released, [&](){
        QString stlFilePath = QFileDialog::getOpenFileName(w, "Open STL Model", "", "STL Files (*.stl)");
        if (!stlFilePath.isEmpty()) {
            vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
            stlReader->SetFileName(stlFilePath.toStdString().c_str());
            stlReader->Update(); // Read STL file

            mapper->SetInputConnection(stlReader->GetOutputPort()); // Connect mapper to STL output
            actor->SetMapper(mapper); // Assign mapper to actor
        }
        show_btn->clicked();
    });

    // ------------------------
    // Connect Display button
    // ------------------------
    QObject::connect(show_btn, &QPushButton::clicked, [&](){
        // NOTE: Setting actor color and point size here will override PLY color if used
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0); 
        actor->GetProperty()->SetPointSize(5);
        renderer->AddActor(actor); // Add actor to renderer
        renderer->Render();        // Render the scene
        renderWidget->renderWindow()->GetInteractor()->Render();
    });

    // ------------------------
    // Connect Clear button
    // ------------------------
    QObject::connect(clear_btn, &QPushButton::released, [&](){
        renderer->RemoveActor(actor); // Remove actor from renderer
        renderer->Render();           // Refresh the scene
        renderWidget->renderWindow()->GetInteractor()->Render();
    });

    // ------------------------
    // Setup VTK interactor (trackball camera) and axes
    // ------------------------
    vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
    if (!renderWidget->renderWindow()->GetInteractor()){
        // If no interactor exists, create a default one
        vtkNew<QVTKInteractor> iren;
        renderWidget->renderWindow()->SetInteractor(iren);
        iren->SetInteractorStyle(style); // Assign interactor style
        setAxesSystem(iren);            // Add orientation axes
    }
    else{
        auto iren = renderWidget->renderWindow()->GetInteractor();
        iren->SetInteractorStyle(style);
        setAxesSystem(iren);            // Add orientation axes
    }

    // ------------------------
    // Show main window
    // ------------------------
    w->resize(800, 800);
    w->show();

    return app.exec(); // Start Qt event loop
}