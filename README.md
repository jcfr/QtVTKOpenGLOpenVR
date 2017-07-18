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

Build instructions
------------------

* Build SDL

```
git clone https://github.com/spurious/SDL-mirror.git
```

* Build OpenVR

```
git clone https://github.com/ValveSoftware/openvr.git
```

* Download Qt5

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

Usage
-----

*To be done*

