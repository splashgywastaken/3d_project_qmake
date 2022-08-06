#ifndef OPTIMIZATION_RIGIDALIGNMENTSCALINGPROBLEM_H
#define OPTIMIZATION_RIGIDALIGNMENTSCALINGPROBLEM_H

#include <problem.h>
#include <qvector2d.h>

namespace Optimization {

class RigidAlignmentScalingProblem : public Problem
{
public:
    RigidAlignmentScalingProblem(
            const QVector<QVector3D>& baseVertices,
            const QVector<QVector3D>& targetVertices
            );

    double computeError(const QVector<double>& variables) override;
    virtual QVector<double> computeGradient(const QVector<double>& variables) override;

    static QMatrix4x4 transformationMatrixFromVars(const QVector<double>& variables);
    static QVector<double> transformationVectorFromVars(const QVector<double>& variables);
private:
    int nVertices() const;

    QVector<QVector3D> m_baseVertices;
    QVector<QVector3D> m_targetVertices;
};

}
#endif
