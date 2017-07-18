

// Qt includes
#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

// VTK includes
// #include <vtkOpenVRRenderWindow.h>
// #include <vtkRenderWindow.h>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QWidget topLevel;
  QHBoxLayout layout(&topLevel);

  // XXX Replace QWidget with QVTKOpenGLWidget + vtkRenderWindow
  QWidget * openGRViewer = new QWidget(&topLevel);
  layout.addWidget(openGRViewer);

  // XXX Replace QWidget with QVTKOpenGLWidget + vtkOpenVRRenderWindow
  QWidget * openVRViewer = new QWidget(&topLevel);
  layout.addWidget(openVRViewer);
  
  topLevel.show();

  return app.exec();
}
