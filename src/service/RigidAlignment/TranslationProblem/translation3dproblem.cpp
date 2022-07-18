#include "translation3dproblem.h"
#include "src/service/Optimization/OptimizationUtils/optimizationutils.h"

namespace Optimization {

Translation3DProblem::Translation3DProblem(
        const QVector<QVector3D> &baseVertices,
        const QVector<QVector3D> &targetVertices
        ) :
    m_baseVertices(baseVertices),
    m_targetVertices(targetVertices)
{
    Q_ASSERT(baseVertices.size() == targetVertices.size());
}

double Translation3DProblem::computeError(const QVector<double> &variables)
{
    const int nVariables = variables.size();
    Q_ASSERT(nVariables == 3);
    const QVector3D translation(variables[0], variables[1], variables[2]);

    double resultError = 0;
    for (int index = 0; index < nVertices(); index++)
    {
        resultError += (m_baseVertices[index] + translation - m_targetVertices[index]).lengthSquared();
    }

    return resultError;
}

QVector<double> Translation3DProblem::computeGradient(const QVector<double> &variables)
{
    const auto function = [&](const QVector<double>& variables) -> double
    {
        return this->computeError(variables);
    };
    const float dx = 1e-5;
    return numericalGradient(function, variables, dx);
}

int Translation3DProblem::nVertices() const
{
    return m_baseVertices.size();
}



}
