cmake_minimum_required(VERSION 3.7)

project(QtVTKOpenGLOpenVR)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

find_package(VTK REQUIRED)
vtk_module_config(VTK
  vtkCommonCore
  vtkGUISupportQt
  vtkInteractionStyle
  vtkRendering${VTK_RENDERING_BACKEND}
  vtkRenderingOpenVR
)
include(${VTK_USE_FILE})

add_executable(experiment
  ctkCallback.cpp
  ctkCallback.h
  main.cpp
  )
target_link_libraries(experiment
  Qt5::Widgets
  ${VTK_LIBRARIES}
  # XXX Explicitly link to fix error related to:
  #     /usr/bin/ld: warning: libSDL2-2.0.so.1, needed by
  #                  /home/jcfr/Projects/VTK-OpenVR-Release/lib/libvtkRenderingOpenVR-8.1.so.1, not found
  #                  (try using -rpath or -rpath-link)
  ${SDL2_LIBRARY}
  )


add_executable(experiment2
  ctkCallback.cpp
  ctkCallback.h
  main2.cpp
  )
target_link_libraries(experiment2
  Qt5::Widgets
  ${VTK_LIBRARIES}
  # XXX Explicitly link to fix error related to:
  #     /usr/bin/ld: warning: libSDL2-2.0.so.1, needed by
  #                  /home/jcfr/Projects/VTK-OpenVR-Release/lib/libvtkRenderingOpenVR-8.1.so.1, not found
  #                  (try using -rpath or -rpath-link)
  ${SDL2_LIBRARY}
  )
