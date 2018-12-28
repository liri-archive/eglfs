find_package(EGL REQUIRED)

liri_add_plugin(eglfs-kms-egldevice-integration
    TYPE
        liri/egldeviceintegrations
    SOURCES
        qeglfskmsegldevice.cpp
        qeglfskmsegldevice.h
        qeglfskmsegldeviceintegration.cpp
        qeglfskmsegldeviceintegration.h
        qeglfskmsegldevicemain.cpp
        qeglfskmsegldevicescreen.cpp
        qeglfskmsegldevicescreen.h
    PUBLIC_DEFINES
        QT_EGL_NO_X11
    LIBRARIES
        Liri::EglFSDeviceIntegration
        Liri::EglFSDeviceIntegrationPrivate
        Liri::EglFSKmsSupport
        Liri::EglFSKmsSupportPrivate
        EGL
)
