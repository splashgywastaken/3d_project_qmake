#include "closestpointsbasedalignmentproblem.h"

#include "src/service/Registration/RegistrationClasses/Registration/registration.h"
#include "src/service/Optimization/OptimizationUtils/optimizationutils.h"
#include <src/service/QVectorTools/QVectorTools.h>
#include <src/service/GeomTools/GeomTools.h>

const double normalizationDiagonal = 2;

Optimization::ClosestPointsBasedAlignmentProblem::ClosestPointsBasedAlignmentProblem(
        const QVector<int> &baseTriangleVertexIndices,
        const QVector<QVector3D> &baseVertices,
        const QVector<QVector3D> &targetVertices,
        const QVector<QVector3D> &targetNormals,
        Registration::ClosestPointsFinder &closestPointsFinder,
        const double errorWeight,
        const double minCosBetweenNormals
        ) :
    m_targetVertices(targetVertices),
    m_baseVertices(baseVertices),
    m_errorWeight(errorWeight)
{
    m_rescaleConstant = normalizationDiagonal / m_baseBboxDiagonal;

    const QVector<int> targetVerticesIndices = closestPointsFinder.findClosestPointsIndices(m_baseVertices);
    QVector<QVector3D> targetVerticesSubsetted = QVectorTools::subset(targetVerticesIndices, m_targetVertices);
    QVector<QVector3D> targetNormalsSubsetted = QVectorTools::subset(targetVerticesIndices, targetNormals);

    m_verticesIndicesToFit = QVectorTools::range(nVertices());
    const QVector<QVector3D> baseNormals = GeomTools::computeNormals(baseTriangleVertexIndices, baseVertices);
    QVector<QVector3D> baseNormalsSubseted = QVectorTools::subset(m_verticesIndicesToFit, baseNormals);

    const QVector<int> filteredCorrespondencesIndices = Registration::filterCorrespondencesByNormals(
                baseNormalsSubseted, targetNormalsSubsetted, minCosBetweenNormals);
    m_verticesIndicesToFit = QVectorTools::subset(filteredCorrespondencesIndices, m_verticesIndicesToFit);
    targetVerticesSubsetted = QVectorTools::subset(filteredCorrespondencesIndices, targetVerticesSubsetted);
}

double Optimization::ClosestPointsBasedAlignmentProblem::computeError(const QVector<double> &variables)
{
    const QMatrix4x4 transformation = transformationMatrixFromVariables(variables);

    const int nVerticesToFit = m_verticesIndicesToFit.size();
    double resultError = 0;
    for (int loopIndex = 0; loopIndex < nVerticesToFit; loopIndex++)
    {
        const int index = m_verticesIndicesToFit[loopIndex];
        const QVector3D newVertex = transformation.map(m_baseVertices[index]);
        resultError += (newVertex * m_rescaleConstant - m_targetVertices[loopIndex] * m_rescaleConstant).lengthSquared();
    }

    return m_errorWeight * resultError;
}

QVector<double> Optimization::ClosestPointsBasedAlignmentProblem::computeGradient(const QVector<double> &variables)
{
    const auto function = [&](const QVector<double>& variables) -> double
    {
        return this->computeError(variables);
    };
    const float dx = 1e-3;
    return numericalGradient(function, variables, dx);
}

const double rotationScale = 10;
const double transformationScale = 1;

QMatrix4x4 Optimization::ClosestPointsBasedAlignmentProblem::transformationMatrixFromVariables(const QVector<double> &variables)
{
    const int nVars = variables.size();
    Q_ASSERT(nVars == 6);

    QMatrix4x4 transformation;
    transformation.setToIdentity();

    const QVector3D translation(QVector3D(variables[0], variables[1], variables[2]));
    transformation.translate(translation);

    transformation.rotate(rotationScale * variables[3], QVector3D(1, 0, 0));
    transformation.rotate(rotationScale * variables[4], QVector3D(0, 1, 0));
    transformation.rotate(rotationScale * variables[5], QVector3D(0, 0, 1));

    return transformation;
}

QVector<double> Optimization::ClosestPointsBasedAlignmentProblem::transformationVectorFromVariables(const QVector<double> &variables)
{
    QVector<double> resultVector = variables;

    for (int index = 0; index < 3; index++)
    {
        resultVector[index] *= transformationScale;
    }
    for (int index = 3; index < 6; index++)
    {
        resultVector[index] *= rotationScale;
    }

    return resultVector;
}

int Optimization::ClosestPointsBasedAlignmentProblem::nVertices() const
{
    return m_baseVertices.size();
}

