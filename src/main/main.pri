!MAIN_PRI {
    CONFIG += MAIN_PRI

    INCLUDEPATH += $$PWD

    HEADERS += \
    $$PWD/main.h

    SOURCES += \
    $$PWD/main.cpp

    include($$PWD/mainwindow/mainwindow.pri)
    include($$PWD/FindNearestPointDialog/FindNearestPointDialog.pri)
    include($$PWD/GradientParamsDialog/GradientParamsDialog.pri)

    # Tests
    include($$PWD/../../tests/tests.pri)

    # Misc. code playground files
    include($$PWD/../../code playground/PlaygroundFiles.pri)
}
