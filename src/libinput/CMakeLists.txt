find_package(Libinput REQUIRED)

if(NOT TARGET Liri::Qt5Udev)
    find_package(Qt5Udev REQUIRED)
endif()
if(NOT TARGET Liri::Logind)
    find_package(Liri1Logind REQUIRED)
endif()

liri_add_module(LibInput
    DESCRIPTION
        "Qt API for libinput"
    SOURCES
        libinputgesture.cpp
        libinputgesture.h
        libinputhandler.cpp
        libinputhandler.h
        libinputhandler_p.h
        libinputkeyboard.cpp
        libinputkeyboard.h
        libinputkeyboard_p.h
        libinputpointer.cpp
        libinputpointer.h
        libinputtouch.cpp
        libinputtouch.h
        logging.cpp
        logging_p.h
    PRIVATE_HEADERS
        libinputhandler_p.h
        libinputkeyboard_p.h
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    LIBRARIES
        eglfsxkb
        Qt5::Core
        Qt5::CorePrivate
        Qt5::Gui
        Qt5::GuiPrivate
        PkgConfig::Libinput
        Liri::Qt5Udev
        Liri::Qt5UdevPrivate
        Liri::Logind
    NO_CMAKE
    NO_PKGCONFIG
    STATIC
)

liri_finalize_module(LibInput)
