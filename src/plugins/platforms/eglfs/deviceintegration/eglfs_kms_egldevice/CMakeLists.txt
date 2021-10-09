find_package(EGL REQUIRED)

if(EGLFS_DRM_ATOMIC)
    set(DEFINES EGLFS_ENABLE_DRM_ATOMIC)
endif()

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
    DEFINES
        ${DEFINES}
    PUBLIC_DEFINES
        QT_EGL_NO_X11
    LIBRARIES
        Liri::EglFSDeviceIntegration
        Liri::EglFSDeviceIntegrationPrivate
        Liri::EglFSKmsSupport
        Liri::EglFSKmsSupportPrivate
        EGL
)

liri_finalize_plugin(eglfs-kms-egldevice-integration)
