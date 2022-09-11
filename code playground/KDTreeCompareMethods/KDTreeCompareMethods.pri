!KDTREECOMPAREMETHODS_PRI {
    CONFIG += KDTREECOMPAREMETHODS_PRI

    INCLUDEPATH += $$PWD

    HEADERS += \
    $$PWD/timetester.h

    SOURCES += \
    $$PWD/timetester.cpp

    include($$PWD/../../src/service/KDTree/KDTree.pri)
    include($$PWD/../../src/models/dto/dto.pri)
    include($$PWD/../../src/service/FileReaders/ObjFileReader/ObjFileReader.pri)
}
