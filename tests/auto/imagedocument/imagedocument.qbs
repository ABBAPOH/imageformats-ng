import qbs.base 1.0

AutoTest {
    Depends { name: "Qt.gui" }
    Depends { name: "Lib" }

    cpp.defines: base.concat(["QT_STATICPLUGIN=1"])

    name: "imagedocument"
    files: [ "*.cpp", "*.h" ]
}
