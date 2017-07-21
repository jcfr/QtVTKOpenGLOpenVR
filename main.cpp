// Qt includes
#include <QApplication>
#include <QPushButton>
#include <QSurfaceFormat>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>

// CTK includes
#include <ctkCallback.h>

// VTK includes
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkCullerCollection.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>
#include <vtkNew.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkOpenGLVertexBufferObject.h>
#include <vtkOpenVRCamera.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>

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
void configureRenderer(vtkRenderer* ren)
{
  ren->SetGradientBackground(1);
  ren->SetBackground2(0.7, 0.7, 0.7);

  vtkNew<vtkConeSource> coneSource;
  coneSource->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(coneSource->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->GetProperty()->SetColor(1, 0, 0);
  actor->SetMapper(mapper.GetPointer());

  ren->AddActor(actor.GetPointer());
  ren->SetBackground(0, 0, 1);
}

//-----------------------------------------------------------------------------
// Adapted from Paraview
// See https://gitlab.kitware.com/paraview/paraview/blob/master/ParaViewCore/ClientServerCore/Rendering/vtkPVRenderView.cxx
void sendToOpenVR(vtkRenderer* srcRenderer, vtkOpenVRRenderWindow* vrRenWin)
{
  vrRenWin->SetMultiSamples(8);
  vtkNew<vtkOpenVRRenderer> vrRen;
  vrRenWin->AddRenderer(vrRen.GetPointer());
  vtkNew<vtkOpenVRRenderWindowInteractor> vrIren;
  vrRenWin->SetInteractor(vrIren.GetPointer());
  vtkNew<vtkOpenVRCamera> vrCam;
  vrRen->SetActiveCamera(vrCam.GetPointer());

  vrRen->RemoveCuller(vrRen->GetCullers()->GetLastItem());
  vrRen->SetBackground(srcRenderer->GetBackground());

  // create 4 lights for even lighting
  {
    vtkLight* light = vtkLight::New();
    light->SetPosition(0.0, 1.0, 0.0);
    light->SetIntensity(1.0);
    light->SetLightTypeToSceneLight();
    vrRen->AddLight(light);
    light->Delete();
  }
  {
    vtkLight* light = vtkLight::New();
    light->SetPosition(0.8, -0.2, 0.0);
    light->SetIntensity(0.8);
    light->SetLightTypeToSceneLight();
    vrRen->AddLight(light);
    light->Delete();
  }
  {
    vtkLight* light = vtkLight::New();
    light->SetPosition(-0.3, -0.2, 0.7);
    light->SetIntensity(0.6);
    light->SetLightTypeToSceneLight();
    vrRen->AddLight(light);
    light->Delete();
  }
  {
    vtkLight* light = vtkLight::New();
    light->SetPosition(-0.3, -0.2, -0.7);
    light->SetIntensity(0.4);
    light->SetLightTypeToSceneLight();
    vrRen->AddLight(light);
    light->Delete();
  }

  vrRenWin->InitializeViewFromCamera(srcRenderer->GetActiveCamera());

  vtkActorCollection* acol = srcRenderer->GetActors();
  vtkCollectionSimpleIterator pit;
  vtkActor* actor;
  for (acol->InitTraversal(pit); (actor = acol->GetNextActor(pit));)
  {
    actor->ReleaseGraphicsResources(NULL);
    vrRen->AddActor(actor);
    // always use shift scale, everyone should
    vtkOpenGLPolyDataMapper* pdm = vtkOpenGLPolyDataMapper::SafeDownCast(actor->GetMapper());
    if (pdm)
    {
      pdm->SetVBOShiftScaleMethod(vtkOpenGLVertexBufferObject::AUTO_SHIFT_SCALE);
    }
  }
  vrRenWin->Initialize();
  if (vrRenWin->GetHMD())
  {
    vrRenWin->Render();
    vrRen->ResetCamera();
    vrIren->Start();
  }
  for (acol->InitTraversal(pit); (actor = acol->GetNextActor(pit));)
  {
    actor->ReleaseGraphicsResources(vrRenWin);
  }
}

//-----------------------------------------------------------------------------
struct CallbackData
{
  CallbackData(vtkRenderer* srcRenderer, vtkOpenVRRenderWindow* vrRenWin):srcRenderer(srcRenderer), vrRenWin(vrRenWin){};
  vtkRenderer* srcRenderer;
  vtkOpenVRRenderWindow* vrRenWin;
};

//-----------------------------------------------------------------------------
void sendToOpenVRCallback(void * data)
{
  CallbackData* cbData = reinterpret_cast<CallbackData*>(data);
  sendToOpenVR(cbData->srcRenderer, cbData->vrRenWin);
}

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{

  // Set default surface format for QVTKOpenGLWidget
  QSurfaceFormat format = QVTKOpenGLWidget::defaultFormat();
  QSurfaceFormat::setDefaultFormat(format);

  // Create application
  QApplication app(argc, argv);
  QWidget topLevel;
  topLevel.setMinimumSize(600, 600);
  QVBoxLayout layout(&topLevel);
  topLevel.show();

  // Create QVTKOpenGLWidget + vtkRenderWindow
  vtkNew<vtkGenericOpenGLRenderWindow> renWin;
  vtkNew<vtkRenderer> ren;
  renWin->AddRenderer(ren.GetPointer());
  configureRenderer(ren.GetPointer());
  QVTKOpenGLWidget *openGRViewer = new QVTKOpenGLWidget(&topLevel);
  openGRViewer->SetRenderWindow(renWin.GetPointer());
  layout.addWidget(openGRViewer);

  QPushButton * sync = new QPushButton("Sync with VR");
  layout.addWidget(sync);

  // Create vtkOpenVRRenderWindow
  vtkNew<vtkOpenVRRenderWindow> vrRenWin;

  QTimer timer;

  // Connection
  ctkCallback callback(sendToOpenVRCallback);
  CallbackData cbData(ren.GetPointer(), vrRenWin.GetPointer());
  callback.setCallbackData(&cbData);
  QObject::connect(sync, SIGNAL(clicked()), &callback, SLOT(invoke()));
  //QObject::connect(&timer, SIGNAL(timeout()), &callback, SLOT(invoke()));

  timer.start(100);

  return app.exec();
}
