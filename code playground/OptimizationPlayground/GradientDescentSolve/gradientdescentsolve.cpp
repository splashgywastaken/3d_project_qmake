#include "gradientdescentsolve.h"

#include <QVector>

#include <lambdaproblem.h>
#include "optimizationutils.h"
#include "qdebug.h"
#include <gradientdescent.h>

void Optimization::GradientDescentSolve::solveMinimizeFunction()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 2);
        double x = vars[0];
        double y = vars[1];

        return x * x + 1.5 * y * y + 0.5 * x * y + 10 * x - 5;
    };

    const auto gradientFunc = [=](const QVector<double> &vars) -> QVector<double> {
        return numericalGradient(errorFunc, vars, 1e-8);
    };
    Optimization::LambdaProblem problem(errorFunc, gradientFunc);

    const QVector<double> initialVariables = {-6, -3};
    const QVector<double> result = Optimization::gradientDescent(problem, initialVariables, 1e-1, 1500, 1e-7, true);

    qDebug() << QString("Result of minimizing function is: (%1;%2)").arg(result[0]).arg(result[1]);
}
