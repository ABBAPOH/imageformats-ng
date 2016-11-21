import qbs.base 1.0

Project {
    SubProject {
        filePath: "app/app.qbs"
    }
    SubProject {
        filePath: "libs/libs.qbs"
    }
    SubProject {
        filePath: "plugins/plugins.qbs"
    }
    SubProject {
        filePath: "tools/tools.qbs"
    }
}

