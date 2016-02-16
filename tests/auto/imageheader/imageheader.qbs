import qbs.base 1.0

AutoTest {
    Depends { name: "Qt.gui" }
    Depends { name: "ImageIO" }

    name: "imageheader"
    files: [ "*.cpp", "*.h" ]
}
