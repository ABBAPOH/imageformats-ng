import qbs.base 1.0

Product {
    property bool isStatic: false
    type: isStatic ? "staticlibrary" : "dynamiclibrary"
    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.gui" }

    destinationDirectory: project.install_library_path
    bundle.isBundle: false

    cpp.cFlags: project.cFlags
    cpp.cxxFlags: project.cxxFlags
    cpp.linkerFlags: project.linkFlags
    cpp.includePaths: project.includePaths
    cpp.libraryPaths: project.libraryPaths
    cpp.cxxLanguageVersion: "c++11"
    cpp.cxxStandardLibrary: qbs.targetOS.contains("osx") ? "libc++" : base
    cpp.minimumOsxVersion: "10.7"
    cpp.installNamePrefix: "@rpath/Frameworks"
    cpp.rpaths: qbs.targetOS.contains("osx")
                ? [ "@loader_path/..", "@executable_path/.." ]
                : [ "$ORIGIN" ]

    Group {
        condition: !isStatic
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_library_path
    }
}
