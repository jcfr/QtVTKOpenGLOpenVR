QtVTKOpenGLOpenVR
=================

Experimenting with creating Qt application synchronizing VTK OpenGL renderer with VTK OpenVR renderer.

Project created while attending the 26th Slicer Project Week taking place at the Robarts Scientific Institute in London, Ontario.

See http://wiki.imaging.robarts.ca/index.php/Main_Page

Overview
--------

The _traditional_ approach consists in both (1) associating every actors of the source renderer with the OpenVR renderer
and (2) starting the OpenVR interactor. This approach prevents the user from updating actors associated with the source
renderer while they are rendered in the OpenVR window.

This project intends to explore few approaches allowing to maintain interactivity with both the OpenGL and OpenVR render
window.

It is understood that at first performance may be degraded. That said, we anticipate that hardware performance will
improve and that this approach will be viable in the near term.

Proposed approaches
-------------------

#### Approach 1: Remove SDL requirements + sync Qt event loop with VR + sync actors

_to be done_



Prerequisites
-------------

* CMake
* Qt5
* SDL 
* OpenVR
* VTK8

### Install prerequisites

* Install CMake

See https://cmake.org/download/

* Install Qt5

See https://download.qt.io/archive/qt/, then download and install the latest version (e.g 5.9.1).

```
wget https://download.qt.io/archive/qt/5.9/5.9.1/qt-opensource-linux-x64-5.9.1.run
chmod u+x qt-opensource-linux-x64-5.9.1.run
```

* Install OpenVR:

```
curl -L https://github.com/ValveSoftware/openvr/archive/v1.0.9.tar.gz -o openvr-v1.0.9.tar.gz
aunpack openvr-v1.0.9.tar.gz
```

### Build prerequisites

* Build SDL

```
git clone https://github.com/spurious/SDL-mirror.git

mkdir -p SDL-install
SDL_DIR=$(pwd)/SDL-install

mkdir -p SDL-build
cd SDL-build

cmake \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  -DCMAKE_INSTALL_PREFIX:PATH=${SDL_DIR} \
  ../SDL-mirror/
make -j4
```

* Build VTK8:

```
git clone git://github.com/kitware/VTK

mkdir -p VTK-OpenVR-build
cd VTK-OpenVR-build

Qt5_DIR=/home/jcfr/Software/Qt5.9.1/5.9.1/gcc_64/lib/cmake/Qt5
OPENVR_DIR=/home/jcfr/Projects/openvr-1.0.9
SDL_DIR=/home/jcfr/Projects/SDL-install/

cmake \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  -DBUILD_TESTING:BOOL=OFF \
  -DModule_vtkGUISupportQt:BOOL=ON \
  -DModule_vtkRenderingOpenVR:BOOL=ON \
  -DVTK_OPENVR_OBJECT_FACTORY:BOOL=OFF \
  -DVTK_QT_VERSION:STRING=5 \
  -DQt5_DIR:PATH=${Qt5_DIR} \
  -DOPENVR_INCLUDE_DIR:PATH=${OPENVR_DIR}/headers/ \
  -DOPENVR_LIBRARY:FILEPATH=${OPENVR_DIR}/bin/linux64/libopenvr_api.so \
  -DSDL2_LIBRARY:FILEPATH=${SDL_DIR}/lib/libSDL2-2.0.so.0.4.1 \
  -DSDL2_INCLUDE_DIR:PATH=${SDL_DIR}/include/SDL2 \
  ../VTK
make -j4
```


Building
--------

* Build QtVTKOpenGLOpenVR

```
mkdir -p QtVTKOpenGLOpenVR-build
cd QtVTKOpenGLOpenVR-build

Qt5_DIR=/home/jcfr/Software/Qt5.9.1/5.9.1/gcc_64/lib/cmake/Qt5
SDL_DIR=/home/jcfr/Projects/SDL-install/
VTK_DIR=/home/jcfr/Projects/VTK-OpenVR-build

cmake \
  -DQt5_DIR:PATH=${Qt5_DIR} \
  -DVTK_DIR:PATH=${VTK_DIR} \
  -DSDL2_LIBRARY:FILEPATH=${SDL_DIR}/lib/libSDL2-2.0.so.0.4.1 \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  ../QtVTKOpenGLOpenVR

make
```

Usage
-----

```
cd QtVTKOpenGLOpenVR-build

SDL_DIR=/home/jcfr/Projects/SDL-install
export LD_LIBRARY_PATH=${SDL_DIR}/lib

./experiment
```

