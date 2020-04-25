eglfs
=====

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/eglfs.svg)](https://github.com/lirios/eglfs)
[![CI](https://github.com/lirios/eglfs/workflows/CI/badge.svg?branch=develop)](https://github.com/lirios/eglfs/actions?query=workflow%3ACI)
[![GitHub issues](https://img.shields.io/github/issues/lirios/eglfs.svg)](https://github.com/lirios/eglfs/issues)

Qt platform plugin with DRM/KMS support.
It's a fork of Qt's eglfs plugin with more feature such as:

 * Get permission to access devices to unprivileged users using logind.
 * Screen configuration.
 * Real-time screen recording.

This plugin extends Qt hence you have to rebuild it every time Qt is updated.

## Dependencies

Qt >= 5.9.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)

And the following modules:

 * [cmake](https://gitlab.kitware.com/cmake/cmake) >= 3.10.0
 * [cmake-shared](https://github.com/lirios/cmake-shared.git) >= 1.0.0
 * [libliri](https://github.com/lirios/libliri.git)
 * [qtudev](https://github.com/lirios/qtudev.git) >= 1.0.0
 * [libinput](http://www.freedesktop.org/wiki/Software/libinput/) >= 0.12
 * [libdrm](https://wiki.freedesktop.org/dri/)
 * [gbm](http://www.mesa3d.org)
 * [egl](http://www.mesa3d.org)

## Installation

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/prefix ..
make
make install # use sudo if necessary
```

Replace `/path/to/prefix` to your installation prefix.
Default is `/usr/local`.

You can also append the following options to the `cmake` command:

 * `-DEGLFS_DRM_ATOMIC:BOOL=OFF`: Disable DRM atomic support.

## Environment variables

The liri QPA plugin supports different graphics hardware through EGL
device integration plugins.

The environment variables that can influence the QPA plugin
are the same as those of [eglfs](http://doc.qt.io/qt-5/embedded-linux.html#eglfs).

## Logging categories

Qt 5.2 introduced logging categories and Liri takes advantage of
them to make debugging easier.

Please refer to the [Qt](http://doc.qt.io/qt-5/qloggingcategory.html) documentation
to learn how to enable them.

## Available categories

* Platform:
  * **liri.qpa.deviceintegration:** Device integration
  * **liri.qpa.input:** Input management on device integration plugins

* EGL Device Integrations:
  * **liri.qpa.kms:** KMS/DRM EGL device integration
  * **liri.qpa.vthandler:** Vt handler

## Licensing

Licensed under the terms of the GNU General Public License version 3.
