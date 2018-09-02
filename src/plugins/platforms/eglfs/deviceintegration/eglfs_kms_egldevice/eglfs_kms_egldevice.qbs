import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "eglfs-kms-egldevice-integration"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "LiriEglFSDeviceIntegration" }
    Depends { name: "LiriEglFSKmsSupport" }

    files: [
        "eglfs_kms_egldevice.json",
        "qeglfskmsegldevice.cpp",
        "qeglfskmsegldevice.h",
        "qeglfskmsegldeviceintegration.cpp",
        "qeglfskmsegldeviceintegration.h",
        "qeglfskmsegldevicemain.cpp",
        "qeglfskmsegldevicescreen.cpp",
        "qeglfskmsegldevicescreen.h",
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
