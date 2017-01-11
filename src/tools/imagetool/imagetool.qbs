import qbs.base 1.0

ConsoleApp {
    Depends { name: "Qt.gui" }
    Depends { name: "ImageFormatsNG" }
    Depends { name: "Utils" }

    files: [
        "*.cpp",
        "abstracttool.h",
        "converttool.h",
        "exception.h",
        "extracttool.h",
        "mainparser.h",
        "showtool.h",
        "toolparser.h",
    ]
}
