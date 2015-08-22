import qbs.base 1.0

App {
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.widgets" }
    Depends { name: "ImageDocument" }
    Depends { name: "ImageView" }
    name: project.app_target
    files: [ "*.cpp", "*.h", "*.ui", "*.qml", "*.qrc", ]
}
