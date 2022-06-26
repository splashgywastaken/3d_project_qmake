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
    src/models/dto/impl/ObjFileData.cpp \
    src/models/3D_obj_data/logo.cpp \
    \
    src/service/file_readers/ObjFileReader/ObjFileReader.cpp \
    src/service/handlers/OpenGL/openglhandler.cpp \
    \
    src/widgets/customglwidget.cpp


HEADERS = \
    src/main/main.h \
    src/main/mainwindow.h \
    \
    src/models/dto/IObjFileData.h \
    src/models/dto/impl/ObjFileData.h \
    src/models/3D_obj_data/logo.cpp \
    \
    src/service/file_readers/ObjFileReader/ObjFileReader.h \
    src/service/handlers/OpenGL/openglhandler.h \
    \
    src/widgets/customglwidget.h


FORMS = \
    src/main/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
