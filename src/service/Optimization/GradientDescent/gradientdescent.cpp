#include "src/service/Optimization/GradientDescent/gradientdescent.h"

#include <QDebug>

QVector<double> Optimization::gradientDescent(
        Optimization::Problem &problem,
        const QVector<double> &initialVars,
        const double stepLength,
        const int nMaxIterations,
        const double gradientNormThreshold,
        const bool verbose,
        StepCallback *callback
        )
{
    QVector<double> currentVariables = initialVars;
        for(int iterationInd = 0; iterationInd < nMaxIterations; ++iterationInd){
            const double error = problem.computeError(currentVariables);
            const QVector<double> gradient = problem.computeGradient(currentVariables);
            const double gradNorm = Optimization::vectorNorm(gradient);
            if(verbose){
                qDebug()
                        << "Iteration:" << iterationInd + 1 << "/" << nMaxIterations
                        << "error:" << error
                        << "gradient norm" << gradNorm;
            }

            if(callback != nullptr)
                callback->call(currentVariables);

            if(gradNorm < gradientNormThreshold){
                if(verbose)
                    qDebug() << "Stopped by gradient norm";
                break;
            }
            const QVector<double> step = mulVectorByScalar(-stepLength, gradient);
            currentVariables = addVectors(currentVariables, step);
        }
        return currentVariables;
}
