!OBJECTVIEWGLWIDGET_PRI {

    CONFIG += OBJECTVIEWGLWIDGET_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../service/globalstate.pri)
    include($$PWD/../../service/EventFilters/CameraMovementEventFilter/CameraMovementEventFilter.pri)

    HEADERS += \
        $$PWD/objectviewglwidget.h

    SOURCES += \
        $$PWD/objectviewglwidget.cpp
}
