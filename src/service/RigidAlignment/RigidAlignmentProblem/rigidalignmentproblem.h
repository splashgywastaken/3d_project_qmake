#ifndef OPTIMIZATION_RIGIDALIGNMENTPROBLEM_H
#define OPTIMIZATION_RIGIDALIGNMENTPROBLEM_H

#include <QVector3D>
#include <src/service/Optimization/Problem/problem.h>
#include <QMatrix4x4>

namespace Optimization {

class RigidAlignmentProblem : public Problem
{
public:
    RigidAlignmentProblem(
            const QVector<QVector3D>& baseVertices,
            const QVector<QVector3D>& targetVertices
            );

    double computeError(const QVector<double>& variables) override;
    virtual QVector<double> computeGradient(const QVector<double>& variables) override;

    static QMatrix4x4 transformationMatrixFromVars(const QVector<double>& variables);
private:
    int nVertices() const;

    QVector<QVector3D> m_baseVertices;
    QVector<QVector3D> m_targetVertices;
};
}

#endif
