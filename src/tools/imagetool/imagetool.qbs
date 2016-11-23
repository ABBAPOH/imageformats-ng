import qbs.base 1.0

ConsoleApp {
    Depends { name: "ImageFormatsNG" }
    files: [
        "*.cpp",
        "abstracttool.h",
        "showtool.h",
    ]
}
