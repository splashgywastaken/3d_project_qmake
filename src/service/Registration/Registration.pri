REGISTRATION_PRI
{
    CONFIG += REGISTRATION_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/ClosestPointsFinder/ClosestPointsFinder.pri)
    include($$PWD/ClosestPointsFinderKDTree/ClosestPointsFinderKDTree.pri)
    include($$PWD/FitterClasses/FitterClasses.pri)
    include($$PWD/RegistrationClasses/RegistrationClasses.pri)
}
