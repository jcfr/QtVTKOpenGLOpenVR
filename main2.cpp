// Qt includes
#include <QApplication>
#include <QDebug>
#include <QEventLoop>
#include <QPushButton>
#include <QSlider>
#include <QSurfaceFormat>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

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
void configureRenderer(vtkRenderer* ren)
{
  vtkNew<vtkSphereSource> sphere;

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(sphere->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper.GetPointer());

  ren->AddActor(actor.GetPointer());
}

void configureVRRenderer(vtkRenderer* vrRen)
{
  vtkNew<vtkOpenVRRenderWindowInteractor> vrIren;
  vrRen->GetRenderWindow()->SetInteractor(vrIren.GetPointer());
  vtkNew<vtkOpenVRCamera> vrCam;
  vrRen->SetActiveCamera(vrCam.GetPointer());

  vrRen->RemoveCuller(vrRen->GetCullers()->GetLastItem());
//  vrRen->SetBackground(srcRenderer->GetBackground());

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
}

//-----------------------------------------------------------------------------
struct CallbackData
{
  CallbackData(vtkRenderWindow* renWin, vtkConeSource* coneSource, QSlider* slider):
    renWin(renWin), coneSource(coneSource), slider(slider){}
  vtkRenderWindow* renWin;
  vtkConeSource* coneSource;
  QSlider* slider;
};

//-----------------------------------------------------------------------------
struct VRCallbackData
{
  VRCallbackData(vtkOpenVRRenderWindow* vrRenWin, vtkRenderer* vrRen, QTimer* vrLoop):
    vrRenWin(vrRenWin), vrRen(vrRen), vrLoop(vrLoop){}
  vtkOpenVRRenderWindow* vrRenWin;
  vtkRenderer* vrRen;
  QTimer* vrLoop;
};

//-----------------------------------------------------------------------------
void startVRCallback(void * data)
{
  VRCallbackData* cbData = reinterpret_cast<VRCallbackData*>(data);

  vtkOpenVRRenderWindow* vrRenWin = cbData->vrRenWin;
  vtkRenderer* vrRen = cbData->vrRen;
  QTimer* vrLoop = cbData->vrLoop;

  //  vrRenWin->InitializeViewFromCamera(srcRenderer->GetActiveCamera());

  //  vtkActorCollection* acol = srcRenderer->GetActors();
  //  vtkCollectionSimpleIterator pit;
  //  vtkActor* actor;
  //  for (acol->InitTraversal(pit); (actor = acol->GetNextActor(pit));)
  //  {
  //    actor->ReleaseGraphicsResources(NULL);
  //    vrRen->AddActor(actor);
  //    // always use shift scale, everyone should
  //    vtkOpenGLPolyDataMapper* pdm = vtkOpenGLPolyDataMapper::SafeDownCast(actor->GetMapper());
  //    if (pdm)
  //    {
  //      pdm->SetVBOShiftScaleMethod(vtkOpenGLVertexBufferObject::AUTO_SHIFT_SCALE);
  //    }
  //  }
    vrRenWin->Initialize();
    if (vrRenWin->GetHMD())
    {
      vrRenWin->Render();
      vrRen->ResetCamera();
      vrLoop->start();
  //    vrRenWin->GetInteractor()->Start();
    }
  //  for (acol->InitTraversal(pit); (actor = acol->GetNextActor(pit));)
  //  {
  //    actor->ReleaseGraphicsResources(vrRenWin);
  //  }
}

//-----------------------------------------------------------------------------
struct VRLoopCallbackData
{
  VRLoopCallbackData(vtkOpenVRRenderWindow* vrRenWin, vtkRenderer* vrRen):
    vrRenWin(vrRenWin), vrRen(vrRen){}
  vtkOpenVRRenderWindow* vrRenWin;
  vtkRenderer* vrRen;
};

//-----------------------------------------------------------------------------
void vrLoopCallback(void * data)
{
  VRLoopCallbackData* cbData = reinterpret_cast<VRLoopCallbackData*>(data);

  vtkOpenVRRenderWindowInteractor * vrIren =
      vtkOpenVRRenderWindowInteractor::SafeDownCast(cbData->vrRenWin->GetInteractor());

  vrIren->DoOneEvent(cbData->vrRenWin, cbData->vrRen);
}

//-----------------------------------------------------------------------------
vtkConeSource* addActor(vtkRenderer* ren)
{
  vtkNew<vtkConeSource> coneSource;
  coneSource->Update();
  //Create a mapper and actor
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(coneSource->GetOutputPort());
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper.GetPointer());
  ren->AddActor(actor);

  return coneSource.GetPointer();
}

//-----------------------------------------------------------------------------
void onHeightChangedCallback(void * data)
{
  CallbackData* cbData = reinterpret_cast<CallbackData*>(data);

  double height = cbData->slider->value() / 100.;

  cbData->coneSource->SetHeight(height);
  cbData->coneSource->Update();
  cbData->renWin->Render();
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
  ren->SetGradientBackground(1);
  ren->SetBackground2(0.7, 0.7, 0.7);
  renWin->AddRenderer(ren.GetPointer());

  vtkConeSource* coneSource = addActor(ren.GetPointer());

  QVTKOpenGLWidget *openGRViewer = new QVTKOpenGLWidget(&topLevel);
  openGRViewer->SetRenderWindow(renWin.GetPointer());
  layout.addWidget(openGRViewer);

  QSlider * slider = new QSlider(Qt::Horizontal);
  slider->setMinimum(10);
  slider->setMaximum(200);
  slider->setValue(100);
  layout.addWidget(slider);

  QPushButton * startVrButton = new QPushButton("Start VR");
  layout.addWidget(startVrButton);

  // Create vtkOpenVRRenderWindow
  vtkNew<vtkOpenVRRenderWindow> vrRenWin;
  vrRenWin->SetMultiSamples(8);
  vtkNew<vtkOpenVRRenderer> vrRen;
  vrRenWin->AddRenderer(vrRen.GetPointer());
  configureVRRenderer(vrRen.GetPointer());
  addActor(vrRen.GetPointer());

  // VR loop timer
  QTimer vrLoop;
  vrLoop.setInterval(10);

  // Connection
  ctkCallback callback(onHeightChangedCallback);
  CallbackData cbData(renWin.GetPointer(), coneSource, slider);
  callback.setCallbackData(&cbData);
  QObject::connect(slider, SIGNAL(valueChanged(int)), &callback, SLOT(invoke()));

  ctkCallback vrCallback(startVRCallback);
  VRCallbackData vrcbData(vrRenWin.GetPointer(), vrRen.GetPointer(), &vrLoop);
  vrCallback.setCallbackData(&vrcbData);
  QObject::connect(startVrButton, SIGNAL(clicked(bool)), &vrCallback, SLOT(invoke()));

  ctkCallback vrLpCallback(vrLoopCallback);
  VRLoopCallbackData vrLoopCbData(vrRenWin.GetPointer(), vrRen.GetPointer());
  vrLpCallback.setCallbackData(&vrLoopCbData);
  QObject::connect(&vrLoop, SIGNAL(timeout()), &vrLpCallback, SLOT(invoke()));

  return app.exec();
}
