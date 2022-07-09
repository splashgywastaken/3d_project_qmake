!MESHTOOLSTESTS_PRI {
    CONFIG += MESHTOOLSTESTS_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../src/service/MeshTools/MeshTools.pri)

    HEADERS += \
    $$PWD/meshtoolstests.h

    SOURCES += \
    $$PWD/meshtoolstests.cpp
}
