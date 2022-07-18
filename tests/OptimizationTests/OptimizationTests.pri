!OPTIMIZATIONTESTS_PRI
{
    CONFIG += OPTIMIZATIONTESTS_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../src/service/Optimization/Optimization.pri)

    HEADERS += $$PWD/optimizationtests.h

    SOURCES += $$PWD/optimizationtests.cpp
}
