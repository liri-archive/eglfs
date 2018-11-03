find_package(Libdrm REQUIRED)
find_package(Gbm REQUIRED)

liri_add_module(EglFSKmsSupport
    DESCRIPTION
        "KMS support for EGL device integration"
    SOURCES
        qeglfskmsdevice.cpp
        qeglfskmsdevice.h
        qeglfskmsintegration.cpp
        qeglfskmsintegration.h
        qeglfskmsscreen.cpp
        qeglfskmsscreen.h
    INSTALL_HEADERS
        qeglfskmsdevice.h
        qeglfskmsintegration.h
        qeglfskmsscreen.h
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
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
    NO_CMAKE
    NO_PKGCONFIG
)
