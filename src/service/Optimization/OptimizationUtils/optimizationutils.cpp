#include "src/service/Optimization/OptimizationUtils/optimizationutils.h"

#include <cmath>

QVector<double> Optimization::addVectors(
        const QVector<double> &a,
        const QVector<double> &b)
{
    const int nItems = a.size();
    Q_ASSERT(b.size() == nItems);
    QVector<double> res(nItems);
    for(int i = 0; i < nItems; ++i)
        res[i] = a[i] + b[i];
    return res;
}

QVector<double> Optimization::subVectors(
        const QVector<double> &a,
        const QVector<double> &b)
{
    const int nItems = a.size();
    Q_ASSERT(b.size() == nItems);
    QVector<double> res(nItems);
    for(int i = 0; i < nItems; ++i)
        res[i] = a[i] - b[i];
    return res;
}

QVector<double> Optimization::mulVectorByScalar(
        const double scalar,
        const QVector<double> &vector)
{
    const int nItems = vector.size();
    QVector<double> res(nItems);
    for(int i = 0; i < nItems; ++i)
        res[i] = scalar * vector[i];
    return res;
}

double Optimization::vectorNorm(const QVector<double> &vector)
{
    const int nItems = vector.size();
    double res = 0;
    for(int i = 0; i < nItems; ++i)
        res += vector[i]*vector[i];
    return sqrt(res);
}
