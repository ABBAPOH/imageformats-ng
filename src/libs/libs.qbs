import qbs.base 1.0

Project {
    SubProject {
        filePath: "3rdparty/3rdparty.qbs"
    }
    SubProject {
        filePath: "imageformatsng/imageformatsng.qbs"
    }
    SubProject {
        filePath: "imageview/imageview.qbs"
    }
    SubProject {
        filePath: "utils/utils.qbs"
    }
}
