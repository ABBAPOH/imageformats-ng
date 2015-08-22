import qbs.base 1.0

Lib {
    Export {
        Depends { name: "cpp" }
        cpp.defines: [ "IMAGEVIEW_EXPORT=Q_DECL_IMPORT" ]
    }

    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.widgets" }
    Depends { name: "ImageDocument" }
    cpp.defines: base.concat(["IMAGEVIEW_EXPORT=Q_DECL_EXPORT"])
    name: "ImageView"
    files: [ "*.cpp", "*.h" ]
}
