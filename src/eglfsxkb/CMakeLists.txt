find_package(XkbCommon REQUIRED)

add_library(eglfsxkb STATIC
    eglfsxkb.cpp
    eglfsxkb.h
)
target_link_libraries(eglfsxkb
    Qt5::Core
    PkgConfig::XkbCommon
)
target_include_directories(eglfsxkb INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
