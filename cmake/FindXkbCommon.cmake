find_package(PkgConfig)

pkg_check_modules(XkbCommon xkbcommon REQUIRED IMPORTED_TARGET)
