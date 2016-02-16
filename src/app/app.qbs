import qbs.base 1.0

App {
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.widgets" }
    Depends { name: "ImageIO" }
    Depends { name: "ImageView" }
    Depends { name: "Utils" }
    name: project.app_target
    files: [ "*.cpp", "*.h", "*.ui", "*.qml", "*.qrc", ]
}
