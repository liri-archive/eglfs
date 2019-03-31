find_package(EGL REQUIRED)

if(EGLFS_DRM_ATOMIC)
    set(DEFINES EGLFS_ENABLE_DRM_ATOMIC)
endif()

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
