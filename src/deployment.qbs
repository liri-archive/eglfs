import qbs 1.0

Project {
    name: "Deployment"

    InstallPackage {
        name: "liri-eglfs-artifacts"
        targetName: name
        builtByDefault: false

        archiver.type: "tar"
        archiver.outputDirectory: project.buildDirectory

        Depends { name: "LiriEglFSDeviceIntegration"; required: false }
        Depends { name: "LiriEglFSDeviceIntegration-qbs"; required: false }
        Depends { name: "LiriPlatformHeaders"; required: false }
        Depends { name: "LiriPlatformHeaders-qbs"; required: false }
        Depends { name: "lirieglfs"; required: false }
        Depends { name: "eglfs-kms-egldevice-integration"; required: false }
        Depends { name: "eglfs-kms-integration"; required: false }
    }
}
