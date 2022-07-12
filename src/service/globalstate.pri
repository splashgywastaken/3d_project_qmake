!GLOBALSTATE_PRI{

    CONFIG += GLOBALSTATE_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/camera/camera.pri)
    include($$PWD/DrawableObjectTools/DrawableObjectTools.pri)
    include($$PWD/FileReaders/FileReaders.pri)
    include($$PWD/MeshTools/meshtools.pri)
    include($$PWD/ProgressNotifier/ProgressNotifier.pri)
}
