import qbs.base 1.0

AutoTest {
    type: "application"
    Depends { name: "Qt.quick" }
    Depends { name: "ImageDocument" }
    Depends { name: "ImageView" }
    name: "qmlapp"
    bundle.isBundle: false
    files: [
        "*.cpp",
        "*.h",
        "*.ui",
        "main.qml",
        "qmlapp.qrc",
    ]
}
