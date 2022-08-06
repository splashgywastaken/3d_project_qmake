#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "src/service/Optimization/Problem/problem.h"
#include "src/service/Optimization/StepCallBack/stepcallback.h"
#include "src/service/Optimization/OptimizationUtils/optimizationutils.h"

namespace Optimization
{

QVector<double> gradientDescent(
        Problem &problem,
        const QVector<double> &initialVars,
        const double stepLength,
        const int nMaxIterations = 100,
        const double gradientNormThreshold = 1e-7,
        const bool verbose = true,
        StepCallback* callback = nullptr
        );

}

#endif
