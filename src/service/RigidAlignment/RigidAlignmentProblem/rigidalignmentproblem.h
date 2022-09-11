#ifndef OPTIMIZATION_RIGIDALIGNMENTPROBLEM_H
#define OPTIMIZATION_RIGIDALIGNMENTPROBLEM_H

#include <QVector3D>
#include <QMatrix4x4>
#include <src/service/Optimization/Problem/problem.h>

namespace Optimization {

class RigidAlignmentProblem : public Problem
{
public:
    RigidAlignmentProblem(
            const QVector<QVector3D>& baseVertices,
            const QVector<QVector3D>& targetVertices
            );

    double computeError(const QVector<double>& variables) override;
    QVector<double> computeGradient(const QVector<double>& variables) override;

    static QMatrix4x4 transformationMatrixFromVars(const QVector<double>& variables);
    static QVector<double> transformationVectorFromVars(const QVector<double>& variables);
private:
    int nVertices() const;

    QVector<QVector3D> m_baseVertices;
    QVector<QVector3D> m_targetVertices;
};
}

#endif
