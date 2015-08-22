import qbs.base 1.0

Project {
    SubProject {
        filePath: "app/app.qbs"
    }
    SubProject {
        filePath: "libs/libs.qbs"
    }
    SubProject {
        filePath: "3rdparty/3rdparty.qbs"
    }
    SubProject {
        filePath: "plugins/plugins.qbs"
    }
}

