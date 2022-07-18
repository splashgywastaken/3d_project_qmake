!LAMBDAPROBLEM_PRI
{
    CONFIG += LAMBDAPROBLEM_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../Problem/Problem.pri)

    HEADERS += $$PWD/lambdaproblem.h

    SOURCES += $$PWD/lambdaproblem.cpp
}
