import qbs.base 1.0

AutoTest {
    Depends { name: "Qt.gui" }
    Depends { name: "ImageIO" }

    cpp.defines: base.concat(["QT_STATICPLUGIN=1"])

    name: "imageio"
    files: [ "*.cpp", "*.h" ]
}
