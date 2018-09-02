eglfs
=====

[![ZenHub.io](https://img.shields.io/badge/supercharged%20by-zenhub.io-blue.svg)](https://zenhub.io)

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/eglfs.svg)](https://github.com/lirios/eglfs)
[![Build Status](https://travis-ci.org/lirios/eglfs.svg?branch=develop)](https://travis-ci.org/lirios/eglfs)
[![GitHub issues](https://img.shields.io/github/issues/lirios/eglfs.svg)](https://github.com/lirios/eglfs/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2018.svg)](https://github.com/lirios/eglfs/commits/develop)

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

 * [qbs-shared](https://github.com/lirios/qbs-shared.git) >= 1.2.0
 * [libliri](https://github.com/lirios/libliri.git)
 * [qtudev](https://github.com/lirios/qtudev.git) >= 1.0.0
 * [libinput](http://www.freedesktop.org/wiki/Software/libinput/) >= 0.12
 * [libdrm](https://wiki.freedesktop.org/dri/)
 * [gbm](http://www.mesa3d.org)

## Installation

Qbs is a new build system that is much easier to use compared to qmake or CMake.

If you want to learn more, please read the [Qbs manual](http://doc.qt.io/qbs/index.html),
especially the [setup guide](http://doc.qt.io/qbs/configuring.html) and how to install artifacts
from the [installation guide](http://doc.qt.io/qbs/installing-files.html).

From the root of the repository, run:

```sh
qbs setup-toolchains --type gcc /usr/bin/g++ gcc
qbs setup-qt /usr/bin/qmake-qt5 qt5
qbs config profiles.qt5.baseProfile gcc
```

Then, from the root of the repository, run:

```sh
qbs -d build -j $(nproc) profile:qt5 # use sudo if necessary
```

To the `qbs` call above you can append additional configuration parameters:

 * `modules.lirideployment.prefix:/path/to/prefix` where most files are installed (default: `/usr/local`)
 * `modules.lirideployment.dataDir:path/to/lib` where data files are installed (default: `/usr/local/share`)
 * `modules.lirideployment.libDir:path/to/lib` where libraries are installed (default: `/usr/local/lib`)
 * `modules.lirideployment.qmlDir:path/to/qml` where QML plugins are installed (default: `/usr/local/lib/qml`)
 * `modules.lirideployment.pluginsDir:path/to/plugins` where Qt plugins are installed (default: `/usr/local/lib/plugins`)
 * `modules.lirideployment.qbsModulesDir:path/to/qbs` where Qbs modules are installed (default: `/usr/local/share/qbs/modules`)

See [lirideployment.qbs](https://github.com/lirios/qbs-shared/blob/develop/modules/lirideployment/lirideployment.qbs)
for more deployment-related parameters.

### Environment variables

The liri QPA plugin supports different graphics hardware through EGL
device integration plugins.

The environment variables that can influence the QPA plugin
are the same as those of [eglfs](http://doc.qt.io/qt-5/embedded-linux.html#eglfs).

### Logging categories

Qt 5.2 introduced logging categories and Liri takes advantage of
them to make debugging easier.

Please refer to the [Qt](http://doc.qt.io/qt-5/qloggingcategory.html) documentation
to learn how to enable them.

### Available categories

* Platform:
  * **liri.qpa.deviceintegration:** Device integration
  * **liri.qpa.input:** Input management on device integration plugins

* EGL Device Integrations:
  * **liri.qpa.kms:** KMS/DRM EGL device integration
  * **liri.qpa.vthandler:** Vt handler

## Licensing

Licensed under the terms of the GNU General Public License version 3.
