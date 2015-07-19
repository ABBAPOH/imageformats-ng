import qbs.base 1.0

Lib {
    Depends { name: "LibPNG" }
    cpp.defines: base.concat(["QT_STATICPLUGIN=1"])
    name: "Lib"
    files: [ "*.cpp", "*.h" ]
}
