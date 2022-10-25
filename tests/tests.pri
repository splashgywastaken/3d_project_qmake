!TESTS_PRI{
    CONFIG += TESTS_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/MeshToolsTests/meshtoolstests.pri)
    include($$PWD/ObjReaderTests/objreadertests.pri)
    include($$PWD/KDTreeTests/KDTreeTests.pri)
    include($$PWD/OptimizationTests/OptimizationTests.pri)
    include($$PWD/GeomToolsTests/GeomToolsTests.pri)
    include($$PWD/QVectorToolsTests/QVectorToolsTests.pri)
    include($$PWD/DialogsToolsTests/dialogstoolstests.pri)
}
