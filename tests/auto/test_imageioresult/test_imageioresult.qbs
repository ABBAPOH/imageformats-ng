import qbs.base 1.0

AutoTest {
    Depends { name: "Qt.gui" }
    Depends { name: "ImageFormatsNG" }

    cpp.defines: base.concat(["QT_STATICPLUGIN=1"])

    files: [ "*.cpp", "*.h" ]
}
