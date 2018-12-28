find_package(Qt5FontDatabaseSupport REQUIRED)
find_package(Qt5ThemeSupport REQUIRED)
find_package(Qt5EventDispatcherSupport REQUIRED)
find_package(Qt5EglSupport REQUIRED)
find_package(Qt5PlatformCompositorSupport REQUIRED)
find_package(Qt5ServiceSupport REQUIRED)
find_package(Qt5FbSupport REQUIRED)
find_package(Fontconfig REQUIRED)

set(QT5PLATFORMSUPPORT_LIBS
    Qt5FontDatabaseSupport::Qt5FontDatabaseSupport
    Qt5ThemeSupport::Qt5ThemeSupport
    Qt5EventDispatcherSupport::Qt5EventDispatcherSupport
    Qt5EglSupport::Qt5EglSupport
    Qt5PlatformCompositorSupport::Qt5PlatformCompositorSupport
    Qt5ServiceSupport::Qt5ServiceSupport
    Qt5FbSupport::Qt5FbSupport
)

if(NOT TARGET Liri::Qt5Udev)
    find_package(Qt5Udev REQUIRED)
endif()
if(NOT TARGET Liri::Logind)
    find_package(Liri1Logind REQUIRED)
endif()

liri_add_module(EglFSDeviceIntegration
    DESCRIPTION
        "EGL device integration"
    SOURCES
        api/libinputmanager.cpp
        api/libinputmanager_p.h
        api/qeglfscontext.cpp
        api/qeglfscontext_p.h
        api/qeglfscursor.cpp
        api/qeglfscursor_p.h
        api/qeglfsdeviceintegration.cpp
        api/qeglfsdeviceintegration_p.h
        api/qeglfsglobal_p.h
        api/qeglfshooks.cpp
        api/qeglfshooks_p.h
        api/qeglfsintegration.cpp
        api/qeglfsintegration_p.h
        api/qeglfslogindhandler.cpp
        api/qeglfslogindhandler_p.h
        api/qeglfsoffscreenwindow.cpp
        api/qeglfsoffscreenwindow_p.h
        api/qeglfsscreen.cpp
        api/qeglfsscreen_p.h
        api/qeglfswindow.cpp
        api/qeglfswindow_p.h
        api/vthandler.cpp
        api/vthandler.h
        api/vthandler_p.h
    PRIVATE_HEADERS
        api/libinputmanager_p.h
        api/qeglfscontext_p.h
        api/qeglfscursor_p.h
        api/qeglfsdeviceintegration_p.h
        api/qeglfshooks_p.h
        api/qeglfsintegration_p.h
        api/qeglfslogindhandler_p.h
        api/qeglfsoffscreenwindow_p.h
        api/qeglfsscreen_p.h
        api/qeglfswindow_p.h
        api/vthandler_p.h
    DEFINES
        QT_NO_CAST_FROM_ASCII
        #QT_NO_FOREACH
    RESOURCES
        cursor.qrc
    PUBLIC_INCLUDE_DIRECTORIES
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>"
    PUBLIC_DEFINES
        QT_EGL_NO_X11
    PUBLIC_LIBRARIES
        Qt5::CorePrivate
        Qt5::GuiPrivate
        Liri::LibInput
        Liri::LibInputPrivate
        Liri::PlatformHeaders
        Liri::Qt5Udev
        Liri::Logind
        ${QT5PLATFORMSUPPORT_LIBS}
    EXPORT_IMPORT_CONDITION
        QT_BUILD_EGL_DEVICE_LIB
    NO_CMAKE
    NO_PKGCONFIG
)

liri_add_plugin(lirieglfs
    TYPE
        platforms
    SOURCES
        qeglfsmain.cpp
    LIBRARIES
        Liri::EglFSDeviceIntegration
        Fontconfig::Fontconfig
)
