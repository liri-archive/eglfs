find_package(EGL REQUIRED)

liri_add_plugin(eglfs-kms-integration
    TYPE
        liri/egldeviceintegrations
    SOURCES
        qeglfskmsgbmcursor.cpp
        qeglfskmsgbmcursor.h
        qeglfskmsgbmdevice.cpp
        qeglfskmsgbmdevice.h
        qeglfskmsgbmintegration.cpp
        qeglfskmsgbmintegration.h
        qeglfskmsgbmmain.cpp
        qeglfskmsgbmscreen.cpp
        qeglfskmsgbmscreen.h
        qeglfskmsgbmwindow.cpp
        qeglfskmsgbmwindow.h
    LIBRARIES
        Liri::EglFSDeviceIntegration
        Liri::EglFSDeviceIntegrationPrivate
        Liri::EglFSKmsSupport
        Liri::EglFSKmsSupportPrivate
        EGL
)
