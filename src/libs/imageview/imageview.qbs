import qbs.base 1.0

Lib {
    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.widgets" }
    Depends { name: "ImageFormatsNG" }
    Depends { name: "Utils" }
    cpp.defines: base.concat(["IMAGEVIEW_LIBRARY"])
    name: "ImageView"
    files: [ "*.cpp", "*.h" ]
}
