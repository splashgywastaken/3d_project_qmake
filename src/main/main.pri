!MAIN_PRI {
CONFIG +=MAIN_PRI

INCLUDEPATH += $$PWD

include($$PWD/../main/main.pri)

HEADERS += \
    $$PWD/mainwindow/mainwindow.h \
    $$PWD/colorpicker/colorpicker.h

SOURCES += \
    $$PWD/mainwindow/mainwindow.cpp \
    $$PWD/colorpicker/colorpicker.cpp

FORMS += \
    $$PWD/mainwindow/mainwindow.ui \
    $$PWD/colorpicker/colorpicker.ui
}
