import qbs.base 1.0

AutoTest {
    Depends { name: "Lib" }

    name: "imagedocument"
    files: [ "*.cpp", "*.h" ]
}
