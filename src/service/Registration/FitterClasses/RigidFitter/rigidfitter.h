#ifndef REGISTRATION_RIGIDFITTER_H
#define REGISTRATION_RIGIDFITTER_H

#include <QMatrix4x4>
#include <src/service/Registration/FitterClasses/Fitter/fitter.h>

namespace Registration {

class RigidFitter : public Fitter
{
public:
    RigidFitter(
            const QVector<QVector3D> &baseVertices,
            const double optimizationStepLength,
            const int optimizationNMaxSteps
            );

    QVector<QVector3D> fit(
            const QVector<int>& verticesToFitIndices,
            const QVector<QVector3D>& verticesToFit,
            const double fitWieght,
            StepCallback* callback = nullptr
            ) override;

    QVector<double> getTransformationVariablesVector() override;

    QMatrix4x4 transformationMatrixFromVariables(const QVector<double>& variables);

    QVector<double> getScaledTransformationVariables(const QVector<double>& variables);

private:
    QVector<QVector3D> m_baseVertices;

    double m_optimizationStepLength = 1e-1;
    int m_optimizationNMaxSteps = 100;

    float m_baseBboxDiagonal = 1.0f;

    const double m_gradThreshold = 1e-7;
    const bool m_verboseOptimization = true;

    QVector<double> m_variableValues = {
        0, 0, 0,
        0, 0, 0
    };
};

}

#endif
