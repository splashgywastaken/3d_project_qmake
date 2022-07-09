!OBJREADERTESTS_PRI{
    CONFIG += OBJREADERTESTS_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../src/service/file_readers/ObjFileReader/ObjFileReader.pri)

    HEADERS += \
    $$PWD/objreadertests.h

    SOURCES += \
    $$PWD/objreadertests.cpp
}
