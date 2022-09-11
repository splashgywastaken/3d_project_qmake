!SERVICE_PRI{

    CONFIG += SERVICE_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/globalstate.pri)
    include($$PWD/EventFilters/EventFilters.pri)
    include($$PWD/StateMachines/StateMachines.pri)
    include($$PWD/KDTree/KDTree.pri)
    include($$PWD/GeomTools/GeomTools.pri)
    include($$PWD/QVectorTools/QVectorTools.pri)
    include($$PWD/Registration/Registration.pri)
}
