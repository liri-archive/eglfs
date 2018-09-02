import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "eglfs-kms-integration"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "LiriEglFSDeviceIntegration" }
    Depends { name: "LiriEglFSKmsSupport" }
    Depends { name: "Qt5Udev" }

    files: [
        "eglfs_kms.json",
        "qeglfskmsgbmcursor.cpp",
        "qeglfskmsgbmcursor.h",
        "qeglfskmsgbmdevice.cpp",
        "qeglfskmsgbmdevice.h",
        "qeglfskmsgbmintegration.cpp",
        "qeglfskmsgbmintegration.h",
        "qeglfskmsgbmmain.cpp",
        "qeglfskmsgbmscreen.cpp",
        "qeglfskmsgbmscreen.h",
        "qeglfskmsgbmwindow.cpp",
        "qeglfskmsgbmwindow.h",
    ]

    FileTagger {
        patterns: "*.json"
        fileTags: ["qt_plugin_metadata"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/liri/egldeviceintegrations"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
