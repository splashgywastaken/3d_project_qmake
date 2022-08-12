#include "registration.h"

#include <QDebug>
#include <src/service/QVectorTools/QVectorTools.h>
#include <src/service/GeomTools/GeomTools.h>

QVector<QVector3D> Registration::performIcp(
        Fitter &fitter,
        ClosestPointsFinder &closestPointsFinder,
        const QVector<int> &baseTriangleVertexIndices,
        const QVector<QVector3D> &initialVertices,
        const QVector<QVector3D> &targetVertices,
        const QVector<QVector3D> &targetNormals,
        const int nIterations,
        const float minCosBetweenNormals,
        const bool verbose,
        StepCallback *stepCallback)
{
    QVector<QVector3D> currentVertices = initialVertices;
    const int nVertices = currentVertices.size();

    const double fitWeight = 1.0;

    for (int iterInd = 0; iterInd < nIterations; ++iterInd)
    {
        if (verbose)
        {
            qDebug() << "ICP Iteration:" << iterInd + 1 << "/" << nIterations;
        }
        const QVector<int> targetVerticesIndices = closestPointsFinder.findClosestPointsIndices(currentVertices);
        QVector<QVector3D> targetVerticesSubseted = QVectorTools::subset(targetVerticesIndices, targetVertices);
        QVector<QVector3D> targetNormalsSubseted = QVectorTools::subset(targetVerticesIndices, targetNormals);

        QVector<int> verticesIndicesToFit = QVectorTools::range(nVertices);
        const QVector<QVector3D> baseNormals = GeomTools::computeNormals(baseTriangleVertexIndices, currentVertices);
        QVector<QVector3D> baseNormalsSubseted = QVectorTools::subset(verticesIndicesToFit, baseNormals);

        const QVector<int> filteredCorrespondencesIndices = filterCorrespondencesByNormals(
                    baseNormalsSubseted, targetNormalsSubseted, minCosBetweenNormals);
        verticesIndicesToFit = QVectorTools::subset(filteredCorrespondencesIndices, verticesIndicesToFit);
        targetVerticesSubseted = QVectorTools::subset(filteredCorrespondencesIndices, targetVerticesSubseted);

        currentVertices = fitter.fit(verticesIndicesToFit, targetVerticesSubseted, fitWeight, stepCallback);
    }

    return currentVertices;
}

QVector<int> Registration::filterCorrespondencesByNormals(
        const QVector<QVector3D> &originalNormals,
        const QVector<QVector3D> &targetNormals,
        const float minCosBetweenNormals)
{
    QVector<int> res;
    const int nVertices = originalNormals.size();
    Q_ASSERT(targetNormals.size() == nVertices);
    for(int i = 0; i < nVertices; ++i){
        if (QVector3D::dotProduct(originalNormals[i], targetNormals[i]) < minCosBetweenNormals)
        {
            continue;
        }
        res << i;
    }
    return res;
}
