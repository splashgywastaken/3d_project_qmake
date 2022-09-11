!OBJREADERTESTS_PRI{
    CONFIG += OBJREADERTESTS_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../src/service/FileReaders/ObjFileReader/ObjFileReader.pri)

    HEADERS += \
    $$PWD/objreadertests.h

    SOURCES += \
    $$PWD/objreadertests.cpp
}
