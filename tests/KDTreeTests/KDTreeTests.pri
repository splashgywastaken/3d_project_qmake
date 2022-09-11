!KDTREETESTS_PRI
{
    CONFIG += KDTREETESTS_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../src/service/KDTree/KDTree.pri)

    HEADERS += \
    $$PWD/kdtreetests.h

    SOURCES += \
    $$PWD/kdtreetests.cpp
}
