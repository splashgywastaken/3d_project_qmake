#include "rigidalignmentproblem.h"
#include "src/service/Optimization/OptimizationUtils/optimizationutils.h"

Optimization::RigidAlignmentProblem::RigidAlignmentProblem(
        const QVector<QVector3D>& baseVertices,
        const QVector<QVector3D>& targetVertices
        ) :
    m_baseVertices(baseVertices),
    m_targetVertices(targetVertices)
{
    Q_ASSERT(baseVertices.size() == targetVertices.size());
}

double Optimization::RigidAlignmentProblem::computeError(const QVector<double> &variables)
{
    const QMatrix4x4 transformation = transformationMatrixFromVars(variables);

    double resultError = 0;
    for (int i = 0; i < nVertices(); i++)
    {
        const QVector3D newVertex = transformation.map(m_baseVertices[i]);
        resultError += (newVertex - m_targetVertices[i]).lengthSquared() / nVertices();
    }

    return resultError;
}

QVector<double> Optimization::RigidAlignmentProblem::computeGradient(const QVector<double> &variables)
{
    const auto function = [&](const QVector<double>& vars) -> double
    {
        return this->computeError(vars);
    };
    const float dx = 1e-3;
    return numericalGradient(function, variables, dx);
}

QMatrix4x4 Optimization::RigidAlignmentProblem::transformationMatrixFromVars(const QVector<double> &variables)
{
    const int nVars = variables.size();
    Q_ASSERT(nVars == 6);
    const double rotationScale = 10;
    QMatrix4x4 transformation;

    const QVector3D translation(variables[0], variables[1], variables[2]);
    transformation.translate(translation);

    transformation.rotate(rotationScale * variables[3], QVector3D(1, 0, 0));
    transformation.rotate(rotationScale * variables[4], QVector3D(0, 1, 0));
    transformation.rotate(rotationScale * variables[5], QVector3D(0, 0, 1));

    return transformation;
}

int Optimization::RigidAlignmentProblem::nVertices() const
{
    return m_baseVertices.size();
}

