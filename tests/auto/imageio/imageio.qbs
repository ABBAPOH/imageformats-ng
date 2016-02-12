import qbs.base 1.0

AutoTest {
    Depends { name: "Qt.gui" }
    Depends { name: "ImageDocument" }

    cpp.defines: base.concat(["QT_STATICPLUGIN=1"])

    name: "imageio"
    files: [ "*.cpp", "*.h" ]
}
