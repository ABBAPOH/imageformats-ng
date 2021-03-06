import qbs.base 1.0

Project {
    StaticLib {
        Depends { name: "cpp" }
        name: "JPEGLib"
        Group {
            name: "files"
            cpp.cFlags: {
                if (qbs.toolchain.contains("mingw"))
                    return base.concat(["-Wno-unused-parameter", "-Wno-main"])
                else if (qbs.toolchain.contains("msvc"))
                    return base
                return base.concat(["-Wno-unused-parameter"])
            }
            files: [
                "jaricom.c",
                "jcapimin.c",
                "jcapistd.c",
                "jcarith.c",
                "jccoefct.c",
                "jccolor.c",
                "jcdctmgr.c",
                "jchuff.c",
                "jcinit.c",
                "jcmainct.c",
                "jcmarker.c",
                "jcmaster.c",
                "jcomapi.c",
                "jcparam.c",
                "jcprepct.c",
                "jcsample.c",
                "jctrans.c",
                "jdapimin.c",
                "jdapistd.c",
                "jdarith.c",
                "jdatadst.c",
                "jdatasrc.c",
                "jdcoefct.c",
                "jdcolor.c",
                "jddctmgr.c",
                "jdhuff.c",
                "jdinput.c",
                "jdmainct.c",
                "jdmarker.c",
                "jdmaster.c",
                "jdmerge.c",
                "jdpostct.c",
                "jdsample.c",
                "jdtrans.c",
                "jerror.c",
                "jfdctflt.c",
                "jfdctfst.c",
                "jfdctint.c",
                "jidctflt.c",
                "jidctfst.c",
                "jidctint.c",
                "jquant1.c",
                "jquant2.c",
                "jutils.c",
                "jmemmgr.c",
                "jmemnobs.c"
            ]
            prefix: "libjpeg/"
        }
        Export {
            Depends { name: "cpp" }
            cpp.includePaths: [ "libjpeg" ]
        }
    }

    StaticLib {
        Depends { name: "cpp" }
        Depends { name: "ZLib" }
        name: "PNGLib"
        files: [
            "libpng/png.c",
            "libpng/pngerror.c",
            "libpng/pngget.c",
            "libpng/pngmem.c",
            "libpng/pngpread.c",
            "libpng/pngread.c",
            "libpng/pngrio.c",
            "libpng/pngrtran.c",
            "libpng/pngrutil.c",
            "libpng/pngset.c",
            "libpng/pngtrans.c",
            "libpng/pngwio.c",
            "libpng/pngwrite.c",
            "libpng/pngwtran.c",
            "libpng/pngwutil.c"
        ]
        Export {
            Depends { name: "ZLib" }
            Depends { name: "cpp" }
            cpp.includePaths: [ "libpng" ]
        }
    }

    Lib {
        Depends { name: "cpp" }
        name: "ZLib"
        files: [
            "zlib/adler32.c",
            "zlib/compress.c",
            "zlib/crc32.c",
            "zlib/deflate.c",
            "zlib/gzclose.c",
            "zlib/gzlib.c",
            "zlib/gzread.c",
            "zlib/gzwrite.c",
            "zlib/infback.c",
            "zlib/inffast.c",
            "zlib/inflate.c",
            "zlib/inftrees.c",
            "zlib/trees.c",
            "zlib/uncompr.c",
            "zlib/zutil.c"
        ]
        Export {
            Depends { name: "cpp" }
            cpp.includePaths: [ "zlib" ]
        }
    }
}
