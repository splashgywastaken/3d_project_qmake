#ifndef PROBLEM_H
#define PROBLEM_H

#include <QVector>

namespace Optimization {

class Problem
{
public:
    virtual double computeError(const QVector<double>& variables) = 0;
    virtual QVector<double> computeGradient(const QVector<double> &variables) = 0;
};

}

#endif
