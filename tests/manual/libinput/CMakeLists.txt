if(NOT TARGET Liri::Logind)
    find_package(Liri1Logind REQUIRED)
endif()

liri_add_executable(tst_libinput
    SOURCES
        tst_libinput.cpp
    LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Test
        Liri::LibInput
        Liri::Logind
    NO_TARGET_INSTALLATION
)
