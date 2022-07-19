!MAINWINDOW_PRI {
    CONFIG += MAINWINDOW_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../widgets/objectviewglwidget/objectviewglwidget.pri)
    include($$PWD/../../models/dto/ObjFileData/ObjFileData.pri)
    include($$PWD/../FindNearestPointDialog/findnearestpointdialog.pri)
    include($$PWD/../../service/RigidAlignment/RigidAlignmentClasses.pri)

    HEADERS += \
    $$PWD/mainwindow.h

    SOURCES += \
    $$PWD/mainwindow.cpp

    FORMS += \
    $$PWD/mainwindow.ui
}
