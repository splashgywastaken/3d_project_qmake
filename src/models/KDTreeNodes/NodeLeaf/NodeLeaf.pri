!NODELEAF_PRI {

    CONFIG += NODELEAF_PRI

    INCLUDEPATH += $$PWD

    HEADERS += \
    $$PWD/nodeleaf.h

    SOURCES += \
    $$PWD/nodeleaf.cpp

    include($$PWD/../Node/Node.pri)
}
