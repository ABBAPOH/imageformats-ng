import qbs.base 1.0

Lib {
    cpp.dynamicLibraries: qbs.targetOS.contains("linux") ? [ "jpeg" ] : []
    name: "Lib"
    files: [ "*.cpp", "*.h" ]

    Group {
        name: "dds"
        files: [ "*.cpp", "*.h" ]
        prefix: "dds/"
    }

    Group {
        name: "jpeg"
        condition: qbs.targetOS.contains("linux")
        files: [ "*.cpp", "*.h" ]
        prefix: "jpeg/"
    }
}
