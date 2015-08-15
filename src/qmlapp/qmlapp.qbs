import qbs.base 1.0

AutoTest {
    type: "application"
    Depends { name: "Qt.quick" }
    Depends { name: "Lib" }
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
