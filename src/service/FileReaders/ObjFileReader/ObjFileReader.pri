!OBJFILEREADER_PRI{
    CONFIG += OBJFILEREADER_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../ProgressNotifier/ProgressNotifier.pri)

    HEADERS += \
        $$PWD/ObjFileReader.h

    SOURCES += \
        $$PWD/ObjFileReader.cpp
}
