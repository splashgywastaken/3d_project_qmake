!TESTS_PRI{
    CONFIG += TESTS_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/MeshToolsTests/meshtoolstests.pri)
    include($$PWD/ObjReaderTests/objreadertests.pri)
    include($$PWD/KDTreeTests/KDTreeTests.pri)
}
