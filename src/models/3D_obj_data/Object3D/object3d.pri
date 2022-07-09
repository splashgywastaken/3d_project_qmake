!OBJECT3D_PRI{
    CONFIG += OBJECT3D_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../../service/globalstate.pri)
    include($$PWD/../../dto/ObjFileData/ObjFileData.pri)

    HEADERS += \
    $$PWD/object3d.h

    SOURCES += \
    $$PWD/object3d.cpp
}
