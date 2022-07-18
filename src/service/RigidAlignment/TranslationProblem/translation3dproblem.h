#ifndef OPTIMIZATION_TRANSLATION3DPROBLEM_H
#define OPTIMIZATION_TRANSLATION3DPROBLEM_H

#include <QVector3D>
#include <src/service/Optimization/Problem/problem.h>

namespace Optimization {

class Translation3DProblem : public Problem
{
public:
    Translation3DProblem(
            const QVector<QVector3D>& baseVertices,
            const QVector<QVector3D>& targetVertices
            );

    double computeError(const QVector<double>& variables) override;
    virtual QVector<double> computeGradient(const QVector<double>& variables) override;

private:
    int nVertices() const;

    QVector<QVector3D> m_baseVertices;
    QVector<QVector3D> m_targetVertices;
};

}

#endif
