import qbs 1.0

QtApplication {
    name: "tst_libinput"

    Depends {
        name: "Qt"
        submodules: ["gui", "gui-private", "testlib"]
        versionAtLeast: project.minimumQtVersion
    }
    Depends { name: "LiriLibInput-private" }
    Depends { name: "LiriLogind" }

    files: [
        "tst_libinput.cpp",
    ]
}
