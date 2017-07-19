// Qt includes
#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

// VTK includes
#include <vtkOpenVRRenderWindow.h>
#include <vtkRenderWindow.h>
#include <QVTKOpenGLWidget.h>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QWidget topLevel;
  topLevel.setMinimumSize(600, 300);

  QHBoxLayout layout(&topLevel);

  // create QVTKOpenGLWidget + vtkRenderWindow
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  QVTKOpenGLWidget *openGRViewer = new QVTKOpenGLWidget(&topLevel);
  openGRViewer->SetRenderWindow(renWin);
  layout.addWidget(openGRViewer);

  // create QVTKOpenGLWidget + vtkOpenVRRenderWindow
  vtkOpenVRRenderWindow *vrRenWin = vtkOpenVRRenderWindow::New();
  QVTKOpenGLWidget *openVRViewer = new QVTKOpenGLWidget(&topLevel);
  openVRViewer->SetRenderWindow(vrRenWin);  
  layout.addWidget(openVRViewer);
  
  topLevel.show();

  return app.exec();
}
