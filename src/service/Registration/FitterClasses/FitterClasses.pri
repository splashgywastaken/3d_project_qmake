FITTERCLASSES_PRI
{
    CONFIG += FITTERCLASSES_PRI
    
    INCLUDEPATH += $$PWD
    
    include($$PWD/Fitter/Fitter.pri)
    include($$PWD/LambdaStepCallback/LambdaStepCallback.pri)
    include($$PWD/StepCallback/StepCallback.pri)
    include($$PWD/RigidFitter/RigidFitter.pri)
}
