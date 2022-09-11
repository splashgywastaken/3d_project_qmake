#ifndef LAMBDAPROBLEM_H
#define LAMBDAPROBLEM_H

#include <functional>
#include <src/service/Optimization/Problem/problem.h>

namespace Optimization {

class LambdaProblem : public Problem
{
public:
    using ErrorFunction = std::function<double(const QVector<double>&)>;
    using GradientFunction = std::function<QVector<double>(const QVector<double>&)>;

    LambdaProblem(
            ErrorFunction errorFunction,
            GradientFunction gradientFunction
            );

    double computeError(const QVector<double>& variables) override;
    QVector<double> computeGradient(const QVector<double> &variables) override;

private:
    ErrorFunction m_errorFunction;
    GradientFunction m_gradientFunction;
};

}

#endif
