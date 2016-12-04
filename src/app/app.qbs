import qbs.base 1.0

Project {
    App {
        Depends { name: "Qt.quick" }
        Depends { name: "Qt.widgets" }
        Depends { name: "ImageFormatsNG" }
        Depends { name: "ImageView" }
        Depends { name: "Utils" }
        name: project.app_target
        files: [ "*.cpp", "*.h", "*.ui", "*.qml", "*.qrc", ]
    }
}
