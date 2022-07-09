QT += core gui testlib opengl openglwidgets

LIBS += -lglu32 -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

# Main and forms
include($$PWD/src/main/main.pri)


# Models
include($$PWD/src/models/models.pri)


# Service and global state
include($$PWD/src/widgets/widgets.pri)
include($$PWD/src/service/service.pri)


# Tests
include($$PWD/tests/tests.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/main/colorpicker/colorpicker.pri \
    src/main/mainwindow/mainwindow.pri \
    src/models/3D_obj_data/3D_obj_data.pri \
    src/models/3D_obj_data/SceneObject/sceneobject.pri \
    src/models/dto/ObjFileData/ObjFileData.pri \
    src/models/dto/dto.pri \
    src/models/models.pri \
    tests/MeshTools/meshtoolstests.pri \
    tests/tests.pri
