liri_add_module(PlatformHeaders
    DESCRIPTION
        "Platform-specific functionality"
    SOURCES
        lirieglfsfunctions.cpp
        lirieglfsfunctions.h
    CLASS_HEADERS
        LiriEglFSFunctions:lirieglfsfunctions.h
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Gui
    PKGCONFIG_DEPENDENCIES
        Qt5Core
        Qt5Gui
)

liri_finalize_module(PlatformHeaders)
