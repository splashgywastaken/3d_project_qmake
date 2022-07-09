!PROGRESS_NOTIFIER_PRI{
    CONFIG += PROGRESS_NOTIFIER_PRI

    INCLUDEPATH += $$PWD

    HEADERS += \
        $$PWD/abstractprogressnotifier.h \
        $$PWD/progressnotifiersingleton.h

    SOURCES += \
        $$PWD/progressnotifiersingleton.cpp
}
