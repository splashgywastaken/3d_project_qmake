!RIGIDALIGNMENTCLASSES_PRI
{
    CONFIG += RIGIDALIGNMENTCLASSES_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/RigidAlignmentScalingProblem/RigidAlignmentScalingProblem.pri)
    include($$PWD/RigidAlignmentProblem/RigidAlignmentProblem.pri)
    include($$PWD/TranslationProblem/TranslationProblem.pri)
    include($$PWD/ClosestPointsBasedAlignmentProblem/ClosestPointsBasedAlignmentProblem.pri)
}
