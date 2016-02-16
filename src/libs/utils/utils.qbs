import qbs.base 1.0

Lib {
    Depends { name: "Qt.gui" }
    cpp.defines: base.concat(["UTILS_LIBRARY"])
    name: "Utils"
    files: [ "*.cpp", "*.h" ]
}
