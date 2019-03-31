cmake_minimum_required(VERSION 3.10.0)

project("EGLFS"
    VERSION "0.0.0"
    DESCRIPTION "EGL full screen Qt platform plugin"
    LANGUAGES CXX C
)

## Shared macros and functions:
if(NOT LIRI_LOCAL_ECM)
    find_package(LiriCMakeShared "1.0.0" REQUIRED NO_MODULE)
    list(APPEND CMAKE_MODULE_PATH "${LCS_MODULE_PATH}")
endif()

## Set minimum versions required:
set(QT_MIN_VERSION "5.9.0")

## Add some paths to check for CMake modules:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty/kwin")

## Liri specific setup common for all modules:
include(LiriSetup)

## Features:
option(EGLFS_DRM_ATOMIC "DRM atomic support" ON)
add_feature_info("EglFS::DrmAtomic" EGLFS_DRM_ATOMIC "Build eglfs with DRM atomic support")

## Find Qt 5:
find_package(Qt5 "${QT_MIN_VERSION}"
    CONFIG REQUIRED
    COMPONENTS
        Core
        DBus
        Gui
        Test
)

## Add subdirectories:
add_subdirectory(src/eglfsxkb)
add_subdirectory(src/libinput)
add_subdirectory(src/platformheaders)
add_subdirectory(src/platformsupport/edid)
add_subdirectory(src/platformsupport/kmsconvenience)
add_subdirectory(src/plugins/platforms/eglfs)
add_subdirectory(src/plugins/platforms/eglfs/deviceintegration/eglfs_kms)
add_subdirectory(src/plugins/platforms/eglfs/deviceintegration/eglfs_kms_egldevice)
add_subdirectory(src/plugins/platforms/eglfs/deviceintegration/eglfs_kms_support)
#add_subdirectory(tests/manual/libinput)
