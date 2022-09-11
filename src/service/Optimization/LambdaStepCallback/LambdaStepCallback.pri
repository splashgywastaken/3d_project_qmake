!LAMBDASTEPCALLBACK_PRI
{
    CONFIG += LAMBDASTEPCALLBACK_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../StepCallback/StepCallback.pri)

    HEADERS += $$PWD/lambdastepcallback.h

    SOURCES += $$PWD/lambdastepcallback.cpp
}
