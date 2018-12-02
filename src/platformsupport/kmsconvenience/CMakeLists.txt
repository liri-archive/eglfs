find_package(Libdrm REQUIRED)

liri_add_module(KmsSupport
    DESCRIPTION
        "KMS shared code"
    SOURCES
        qkmsdevice.cpp
        qkmsdevice_p.h
    PRIVATE_HEADERS
        qkmsdevice_p.h
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    LIBRARIES
        Qt5::Core
        Qt5::CorePrivate
        Qt5::Gui
        Qt5::GuiPrivate
        PkgConfig::Libdrm
    NO_CMAKE
    NO_PKGCONFIG
    STATIC
)
