import qbs 1.0
import LiriUtils

LiriModuleProject {
    name: "LiriEdidSupport"
    moduleName: "LiriEdidSupport"
    description: "EDID parser for EGL device integration"
    createPkgConfig: false
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core-private") }],
    })

    LiriHeaders {
        Group {
            name: "Headers"
            files: "**/*.h"
            fileTags: ["hpp_syncable"]
        }
    }

    LiriPrivateModule {
        targetName: project.targetName

        Depends { name: project.headersName }
        Depends { name: "Qt.core-private" }

        cpp.defines: base.concat([
            'LIRIEGLFS_VERSION="' + project.version + '"',
        ])

        files: [
            "qedidparser.cpp",
            "qedidparser_p.h",
            "qedidvendortable_p.h",
        ]

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends { name: "Qt.core-private" }

            cpp.defines: project.defines
            cpp.includePaths: product.sourceDirectory
        }
    }
}
