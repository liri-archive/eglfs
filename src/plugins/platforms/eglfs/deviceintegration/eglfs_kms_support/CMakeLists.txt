find_package(Libdrm REQUIRED)
find_package(Gbm REQUIRED)
find_package(EGL REQUIRED)

if(EGLFS_DRM_ATOMIC)
    set(DEFINES EGLFS_ENABLE_DRM_ATOMIC)
endif()

liri_add_module(EglFSKmsSupport
    DESCRIPTION
        "KMS support for EGL device integration"
    SOURCES
        qeglfskmsdevice.cpp
        qeglfskmsdevice.h
        qeglfskmshelpers.h
        qeglfskmsintegration.cpp
        qeglfskmsintegration.h
        qeglfskmsscreen.cpp
        qeglfskmsscreen.h
    INSTALL_HEADERS
        qeglfskmsdevice.h
        qeglfskmshelpers.h
        qeglfskmsintegration.h
        qeglfskmsscreen.h
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        ${DEFINES}
    PUBLIC_DEFINES
        QT_EGL_NO_X11
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::CorePrivate
        Qt5::Gui
        Qt5::GuiPrivate
        Liri::EglFSDeviceIntegration
        Liri::EglFSDeviceIntegrationPrivate
        Liri::EdidSupport
        Liri::EdidSupportPrivate
        Liri::KmsSupport
        Liri::KmsSupportPrivate
        PkgConfig::Libdrm
        PkgConfig::Gbm
        EGL
    NO_CMAKE
    NO_PKGCONFIG
)

liri_finalize_module(EglFSKmsSupport)
