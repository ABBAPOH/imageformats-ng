import qbs.base 1.0

App {
    Depends { name: "Qt.widgets" }
    Depends { name: "ImageDocument" }
    name: project.app_target
    files: [ "*.cpp", "*.h", "*.ui" ]
}
