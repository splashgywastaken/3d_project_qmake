!SERVICE_PRI{

    CONFIG += SERVICE_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/globalstate.pri)
    include($$PWD/EventFilters/EventFilters.pri)
    include($$PWD/StateMachines/StateMachines.pri)
}
