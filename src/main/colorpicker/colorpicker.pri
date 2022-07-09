!COLORPICKER_PRI {
    CONFIG += COLORPICKER_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../mainwindow/mainwindow.pri)

    HEADERS += \
    $$PWD/colorpicker.h

    SOURCES += \
    $$PWD/colorpicker.cpp

    FORMS += \
    $$PWD/colorpicker.ui
}
