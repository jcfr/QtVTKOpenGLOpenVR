// Qt includes
#include <QApplication>
#include <QHBoxLayout>
#include <QSurfaceFormat>
#include <QWidget>

// VTK includes
#include <vtkActor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkOpenVRCamera.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>

// XXX To avoid error like the following, "QVTKOpenGLWidget.h" *MUST* be included after "vtkOpenVRRenderWindow.h"
//
//     Error:
//
//       8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---
//       In file included from /path/to/VTK-OpenVR-Release/ThirdParty/glew/vtk_glew.h:53:0,
//                        from /path/to/VTK/Rendering/OpenVR/vtkOpenVRRenderWindow.h:50,
//                        from /path/to/QtVTKOpenGLOpenVR/main.cpp:11:
//       /path/to/VTK/ThirdParty/glew/vtkglew/include/GL/glew.h:85:2: error: #error gl.h included before glew.h
//        #error gl.h included before glew.h
//         ^
//       /path/to/VTK/ThirdParty/glew/vtkglew/include/GL/glew.h:97:2: error: #error glext.h included before glew.h
//        #error glext.h included before glew.h
//         ^
//       /path/to/VTK/ThirdParty/glew/vtkglew/include/GL/glew.h:109:0: warning: "__glext_h_" redefined
//        #define __glext_h_
//        ^
//       In file included from /path/to/Qt5.9.1/5.9.1/gcc_64/include/QtGui/qopengl.h:144:0,
//                        from /path/to/Qt5.9.1/5.9.1/gcc_64/include/QtWidgets/qopenglwidget.h:49,
//                        from /path/to/Qt5.9.1/5.9.1/gcc_64/include/QtWidgets/QOpenGLWidget:1,
//                        from /path/to/VTK/GUISupport/Qt/QVTKOpenGLWidget.h:109,
//                        from /path/to/QtVTKOpenGLOpenVR/main.cpp:10:
//       /path/to/Qt5.9.1/5.9.1/gcc_64/include/QtGui/qopenglext.h:2:0: note: this is the location of the previous definition
//        #define __glext_h_ 1
//       8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---
//
#include <QVTKOpenGLWidget.h>

//-----------------------------------------------------------------------------
void configureRenderWindow(vtkRenderWindow* window)
{
  vtkNew<vtkRenderer> ren;
  ren->SetGradientBackground(1);
  ren->SetBackground2(0.7, 0.7, 0.7);
  window->AddRenderer(ren.GetPointer());

  vtkNew<vtkSphereSource> sphere;

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(sphere->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper.GetPointer());

  ren->AddActor(actor.GetPointer());
}

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{

  // Set default surface format for QVTKOpenGLWidget
  QSurfaceFormat format = QVTKOpenGLWidget::defaultFormat();
  QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);

  QWidget topLevel;
  topLevel.setMinimumSize(600, 300);

  QHBoxLayout layout(&topLevel);

  // create QVTKOpenGLWidget + vtkRenderWindow
  vtkNew<vtkGenericOpenGLRenderWindow> renWin;
  configureRenderWindow(renWin.GetPointer());
  QVTKOpenGLWidget *openGRViewer = new QVTKOpenGLWidget(&topLevel);
  openGRViewer->SetRenderWindow(renWin.GetPointer());
  layout.addWidget(openGRViewer);
  topLevel.show();

  // create vtkOpenVRRenderWindow
  vtkNew<vtkOpenVRRenderWindow> vrRenWin;
  vrRenWin->SetMultiSamples(0);
  vtkNew<vtkOpenVRRenderer> vrRen;
  vrRenWin->AddRenderer(vrRen.GetPointer());
  vtkNew<vtkOpenVRRenderWindowInteractor> vrIren;
  vrRenWin->SetInteractor(vrIren.GetPointer());
  vtkNew<vtkOpenVRCamera> vrCam;
  vrRen->SetActiveCamera(vrCam.GetPointer());

  return app.exec();
}
