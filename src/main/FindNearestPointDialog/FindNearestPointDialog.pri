!FINDNEARESTPOINT_PRI {
    CONFIG += \
    FINDNEARESTPOINT_PRI

    INCLUDEPATH += \
    $$PWD

    include($$PWD/../../service/KDTree/KDTree.pri)

    FORMS += \
        $$PWD/findnearestpointdialog.ui

    HEADERS += \
        $$PWD/findnearestpointdialog.h

    SOURCES += \
        $$PWD/findnearestpointdialog.cpp
}

DISTFILES += \
    $$PWD/FindNearestPointDialog.qss

