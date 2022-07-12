!PROGRESSNOTIFIER_PRI{
    CONFIG += PROGRESSNOTIFIER_PRI

    INCLUDEPATH += $$PWD

    HEADERS += \
        $$PWD/abstractprogressnotifier.h \
        $$PWD/progressnotifiersingleton.h

    SOURCES += \
        $$PWD/progressnotifiersingleton.cpp
}
