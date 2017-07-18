cmake_minimum_required(VERSION 3.8)

project(QtVTKOpenGLOpenVR)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
#find_package(VTK REQUIRED)

add_executable(experiment main.cpp)
target_link_libraries(experiment
  Qt5::Widgets
  )

