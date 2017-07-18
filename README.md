QtVTKOpenGLOpenVR
=================

Experimenting with creating Qt application embedding both OpenGL and OpenVR render windows.

Project created while attending the 26th Slicer Project Week taking place at the Robarts Scientific Institute in London, Ontario.

See http://wiki.imaging.robarts.ca/index.php/Main_Page


Prerequisites
-------------

* Qt5
* VTK8
* OpenVR
* SDL 

### Install prerequisites

* Download Qt5

### Build prerequisites

* Build SDL

```
git clone https://github.com/spurious/SDL-mirror.git
```

* Build OpenVR

```
git clone https://github.com/ValveSoftware/openvr.git
```

* Build VTK8:

```
git clone git://github.com/kitware/VTK
mkdir VTK8-build && cd $_
cmake \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  -DBUILD_TESTING:BOOL=OFF \
  ../VTK
make -j4
```


Building
--------

* Build QtVTKOpenGLOpenVR

```
mkdir QtVTKOpenGLOpenVR-build && cd $_

cmake \
  -DQt5:PATH=/path/to/Qt5.7.1/5.7/gcc_64/lib/cmake/Qt5 \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  ../QtVTKOpenGLOpenVR

make
```

Usage
-----

```
cd QtVTKOpenGLOpenVR-build
./experiment
```

