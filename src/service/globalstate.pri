!GLOBALSTATE_PRI{

    CONFIG += GLOBALSTATE_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/camera/camera.pri)
    include($$PWD/DrawableObjectTools/DrawableObjectTools.pri)
    include($$PWD/file_readers/file_readers.pri)
    include($$PWD/MeshTools/meshtools.pri)
    include($$PWD/progress_notifier/progress_notifier.pri)
}
