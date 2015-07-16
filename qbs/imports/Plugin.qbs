import qbs.base 1.0

DynamicLibrary {
    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.gui" }

    destinationDirectory: project.install_plugin_path + "/imageformats2"
    bundle.isBundle: false

    cpp.cFlags: project.cFlags
    cpp.cxxFlags: project.cxxFlags
    cpp.linkerFlags: project.linkFlags
    cpp.includePaths: project.includePaths
    cpp.libraryPaths: project.libraryPaths
    cpp.cxxLanguageVersion: "c++11"

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.installNamePrefix: "@executable_path/../Frameworks/"
        cpp.minimumOsxVersion: "10.7"
    }

    Properties {
        condition: qbs.targetOS.contains("unix") && !qbs.targetOS.contains("osx")
        cpp.rpaths: "$ORIGIN"
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_plugin_path + "/imageformats2"
    }
}
