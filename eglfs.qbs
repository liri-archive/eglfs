import qbs 1.0

Project {
    name: "EGLFS"

    readonly property string version: "0.0.0"
    readonly property var versionParts: version.split('.').map(function(part) { return parseInt(part); })

    readonly property string minimumQtVersion: "5.9.0"

    property bool useStaticAnalyzer: false
    property bool autotestEnabled: false
    property stringList autotestArguments: []
    property stringList autotestWrapper: []

    condition: qbs.targetOS.contains("linux") && !qbs.targetOS.contains("android")

    minimumQbsVersion: "1.8.0"

    references: [
        "src/deployment.qbs",
        "src/eglfsxkb/eglfsxkb.qbs",
        "src/libinput/libinput.qbs",
        "src/platformheaders/platformheaders.qbs",
        "src/platformsupport/edid/edid.qbs",
        "src/platformsupport/kmsconvenience/kmsconvenience.qbs",
        "src/plugins/platforms/eglfs/eglfs.qbs",
        "tests/manual/manual.qbs",
    ]
}
