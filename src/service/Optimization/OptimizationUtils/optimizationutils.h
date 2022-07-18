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
    const int nVariables = variableValue.size();
    QVector<double> result(nVariables);
    QVector<double> currentVars = variableValue;

    for (int variableIndex = 0; variableIndex < nVariables; variableIndex++)
    {
        currentVars[variableIndex] = variableValue[variableIndex] + dx;
        const double positiveValue = function(currentVars);
        currentVars[variableIndex] = variableValue[variableIndex] - dx;
        const double negativeValue = function(currentVars);
        result[variableIndex] = (positiveValue - negativeValue) / (2 * dx);
    }

    return result;
}

#endif
