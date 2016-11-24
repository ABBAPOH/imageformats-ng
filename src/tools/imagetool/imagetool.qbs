import qbs.base 1.0

ConsoleApp {
    Depends { name: "Qt.gui" }
    Depends { name: "ImageFormatsNG" }
    Depends { name: "Utils" }

    files: [
        "*.cpp",
        "abstracttool.h",
        "showtool.h",
    ]
}
