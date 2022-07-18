#ifndef OPTIMIZATIONUTILS_H
#define OPTIMIZATIONUTILS_H

#include <QVector>

namespace Optimization
{

    QVector<double> addVectors(
            const QVector<double>& a,
            const QVector<double>& b
            );

    QVector<double> subVectors(
            const QVector<double>& a,
            const QVector<double>& b
            );

    QVector<double> mulVectorByScalar(
            const double scalar,
            const QVector<double> &vector
            );

    double vectorNorm(const QVector<double>& vector);

    template<typename Function>
    QVector<double> numericalGradient(
            Function function,
            const QVector<double>& variableValue,
            const double dx = 1e-8
            );
}

template<typename Function>
QVector<double> Optimization::numericalGradient(
        Function function,
        const QVector<double> &variableValue,
        const double dx)
{
    const int nVars = variableValue.size();
    QVector<double> res(nVars);
    QVector<double> currentVars = variableValue;

    Q_UNUSED(function);
    Q_UNUSED(dx);
    Q_UNUSED(currentVars);

    /// Реализуйте расчет численного градиента здесь
    /// Для того, чтобы вычислить значение функции в точке достаточно написать function(vars)
    /// Где vars - значения переменных

    return res;
}

#endif
