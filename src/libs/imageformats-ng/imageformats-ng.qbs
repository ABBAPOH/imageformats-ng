import qbs.base 1.0

Lib {
    Depends { name: "Qt.gui" }
    Depends { name: "Utils" }
    Depends { name: "PNGLib" }
    cpp.defines: base.concat(["QT_STATICPLUGIN=1", "IMAGEIO_LIBRARY"])
    name: "ImageFormatsNG"
    files: [ "*.cpp", "*.h" ]
}
