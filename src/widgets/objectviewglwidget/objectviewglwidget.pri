!OBJECTVIEWGLWIDGET_PRI {

    CONFIG += OBJECTVIEWGLWIDGET_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../service/globalstate.pri)
    include($$PWD/../../service/EventFilters/CameraMovementEventFilter/CameraMovementEventFilter.pri)
    include($$PWD/../../../res/shaders/shaders.pri)

    HEADERS += \
        $$PWD/objectviewglwidget.h

    SOURCES += \
        $$PWD/objectviewglwidget.cpp
}
