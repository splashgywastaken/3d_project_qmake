!SCENE_OBJECT_PRI{
    CONFIG += SCENE_OBJECT_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../../service/globalstate.pri)

    HEADERS += \
    $$PWD/sceneobject.h

    SOURCES += \
    $$PWD/sceneobject.cpp
}
