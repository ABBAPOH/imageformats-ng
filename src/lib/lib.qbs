import qbs.base 1.0

Lib {
    cpp.dynamicLibraries: [ "jpeg" ]
    name: "Lib"
    files: [ "*.cpp", "*.h", "*/*.cpp", "*/*.h" ]
}
