#ifndef CLOSESTPOINTSBASEDALIGNMENTPROBLEM_H
#define CLOSESTPOINTSBASEDALIGNMENTPROBLEM_H

#include <QVector3D>
#include <QMatrix4x4>
#include <src/service/Optimization/Problem/problem.h>
#include <src/service/Registration/ClosestPointsFinder/closestpointsfinder.h>

namespace Optimization
{
class ClosestPointsBasedAlignmentProblem : public Problem
{
public:
    ClosestPointsBasedAlignmentProblem
        (
            const QVector<int> &baseTriangleVertexIndices,
            const QVector<QVector3D> &baseVertices,
            const QVector<QVector3D> &targetVertices,
            const QVector<QVector3D> &targetNormals,
            Registration::ClosestPointsFinder &closestPointsFinder,
            const double errorWeight = 1.0f,
            const double minCosBetweenNormals = 0.3f
        );
    virtual ~ClosestPointsBasedAlignmentProblem() = default;

    double computeError(const QVector<double>& variables) override;
    QVector<double> computeGradient(const QVector<double>& variables) override;

    static QMatrix4x4 transformationMatrixFromVariables(const QVector<double>& variables);
    static QVector<double> transformationVectorFromVariables(const QVector<double>& variables);
private:
    int nVertices() const;

    QVector<QVector3D> m_targetVertices;
    QVector<QVector3D> m_baseVertices;
    QVector<int> m_verticesIndicesToFit;
    double m_errorWeight;
    double m_rescaleConstant;
    double m_baseBboxDiagonal = 1.0f;
};
}

#endif
