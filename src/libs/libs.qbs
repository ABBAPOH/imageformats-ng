import qbs.base 1.0

Project {
    SubProject {
        filePath: "3rdparty/3rdparty.qbs"
    }
    SubProject {
        filePath: "imagedocument/imagedocument.qbs"
    }
    SubProject {
        filePath: "imageview/imageview.qbs"
    }
}
