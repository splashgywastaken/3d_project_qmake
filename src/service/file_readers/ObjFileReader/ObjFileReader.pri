!OBJFILEREADER_PRI{
    CONFIG += OBJFILEREADER_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../progress_notifier/progress_notifier.pri)

    HEADERS += \
        $$PWD/ObjFileReader.h

    SOURCES += \
        $$PWD/ObjFileReader.cpp
}
