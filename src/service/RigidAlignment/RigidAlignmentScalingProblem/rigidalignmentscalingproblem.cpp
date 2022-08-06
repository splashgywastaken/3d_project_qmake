#include "rigidalignmentscalingproblem.h"
#include "optimizationutils.h"
#include "qmatrix4x4.h"

#include <qmath.h>

Optimization::RigidAlignmentScalingProblem::RigidAlignmentScalingProblem(
        const QVector<QVector3D>& baseVertices,
        const QVector<QVector3D>& targetVertices
        ) :
    m_baseVertices(baseVertices),
    m_targetVertices(targetVertices)
{
    Q_ASSERT(baseVertices.size() == targetVertices.size());
}

double Optimization::RigidAlignmentScalingProblem::computeError(const QVector<double> &variables)
{
    const QMatrix4x4 transformation = transformationMatrixFromVars(variables);

    double resultError = 0;
    for (int i = 0; i < nVertices(); i++)
    {
        const QVector3D newVertex = transformation.mapVector(m_baseVertices[i]);
        resultError += (newVertex - m_targetVertices[i]).lengthSquared() / nVertices();
    }

    return resultError;
}

QVector<double> Optimization::RigidAlignmentScalingProblem::computeGradient(const QVector<double> &variables)
{
    const auto function = [&](const QVector<double>& vars) -> double
    {
        return this->computeError(vars);
    };
    const float dx = 1e-3;
    return numericalGradient(function, variables, dx);
}

const double rotationScale = 10;
const double translationScale = 1;
const double scalingScale = 0.5;

QMatrix4x4 Optimization::RigidAlignmentScalingProblem::transformationMatrixFromVars(const QVector<double> &variables)
{
    const int nVars = variables.size();
    Q_ASSERT(nVars == 7 || nVars == 9);
    QMatrix4x4 transformation;

    if (nVars == 7)
    {
       transformation.scale(scalingScale * variables[6]);
    }
    else if (nVars == 9)
    {
        transformation.scale(
                scalingScale * std::abs(variables[6]),
                scalingScale * std::abs(variables[7]),
                scalingScale * std::abs(variables[8])
                );
    }

    const QVector3D translation(
                QVector3D(translationScale * variables[0],translationScale * variables[1],translationScale * variables[2])
            );
    transformation.translate(translation);

    transformation.rotate(rotationScale * variables[3], QVector3D(1, 0, 0));
    transformation.rotate(rotationScale * variables[4], QVector3D(0, 1, 0));
    transformation.rotate(rotationScale * variables[5], QVector3D(0, 0, 1));

    return transformation;
}

QVector<double> Optimization::RigidAlignmentScalingProblem::transformationVectorFromVars(const QVector<double> &variables)
{
    int nVariables = variables.size();

    Q_ASSERT(nVariables == 7 || nVariables == 9);

    QVector<double> resultVector = variables;

    for (int index = 0; index < 3; index++)
    {
        resultVector[index] *= translationScale;
    }
    for (int index = 3; index < 6; index++)
    {
        resultVector[index] *= rotationScale;
    }
    if (nVariables == 9)
    {
        for (int index = 6; index < 6; index++)
        {
            resultVector[index] *= scalingScale;
        }
    }
    else if (nVariables == 7)
    {
        resultVector[6] *= scalingScale;
    }

    return resultVector;
}

int Optimization::RigidAlignmentScalingProblem::nVertices() const
{
    return m_baseVertices.size();
}
