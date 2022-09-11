!NODEINNER_PRI {

    CONFIG += NODEINNER_PRI

    INCLUDEPATH += $$PWD

    HEADERS += \
    $$PWD/nodeinner.h

    SOURCES += \
    $$PWD/nodeinner.cpp

    include($$PWD/../Node/Node.pri)
}
