import qbs.base 1.0

App {
    Depends { name: "Qt.widgets" }
    Depends { name: "Lib" }
    name: project.app_target
    files: [ "*.cpp", "*.h", "*.ui" ]
}
