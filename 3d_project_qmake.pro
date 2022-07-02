QT += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES = \
    src/main/main.cpp \
    src/main/mainwindow.cpp \
    \
    src/models/3D_obj_data/object3d.cpp \
    src/models/dto/ObjFileData/ObjFileData.cpp \
    src/models/3D_obj_data/logo.cpp \
    \
    src/service/file_readers/ObjFileReader/ObjFileReader.cpp \
    \
    src/widgets/customglwidget/customglwidget.cpp \
    src/widgets/objectviewglwidget/objectviewglwidget.cpp


HEADERS = \
    src/main/main.h \
    src/main/mainwindow.h \
    \
    src/models/3D_obj_data/object3d.h \
    src/models/dto/ObjFileData/ObjFileData.h \
    src/models/3D_obj_data/logo.cpp \
    \
    src/service/file_readers/ObjFileReader/ObjFileReader.h \
    \
    src/widgets/customglwidget/customglwidget.h \
    src/widgets/objectviewglwidget/objectviewglwidget.h


FORMS = \
    src/main/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
