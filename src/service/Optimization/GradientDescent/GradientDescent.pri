!GRADIENTDESCENT_PRI
{
    CONFIG += GRADIENTDESCENT_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../LambdaProblem/LambdaProblem.pri)
    include($$PWD/../LambdaStepCallback/LambdaStepCallback.pri)
    include($$PWD/../OptimizationUtils/OptimizationUtils.pri)

    HEADERS += $$PWD/gradientdescent.h

    SOURCES += $$PWD/gradientdescent.cpp
}
