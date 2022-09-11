#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <src/service/Registration/FitterClasses/Fitter/fitter.h>
#include <src/service/Registration/FitterClasses/StepCallback/stepcallback.h>
#include <src/service/Registration/ClosestPointsFinder/closestpointsfinder.h>

namespace Registration {

QVector<int> filterCorrespondencesByNormals(
        const QVector<QVector3D> &originalNormals,
        const QVector<QVector3D> &targetNormals,
        const float minCosBetweenNormals);

QVector<QVector3D> performIcp(
        Fitter &fitter,
        ClosestPointsFinder &closestPointsFinder,
        const QVector<int> &baseTriangleVertexIndices,
        const QVector<QVector3D> &initialVertices,
        const QVector<QVector3D> &targetVertices,
        const QVector<QVector3D> &targetNormals,
        const int nIterations,
        const float minCosBetweenNormals = 0.3f,
        const bool verbose = true,
        StepCallback *stepCallback = nullptr);

}

#endif
