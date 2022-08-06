#include "src/service/Optimization/GradientDescent/gradientdescent.h"

#include <QDebug>
#include <rigidalignmentscalingproblem.h>
#include "rigidalignmentproblem.h"

const double epsilonHard = 1e-7;
double previousError = 0;

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
        for (int iterationInd = 0; iterationInd < nMaxIterations; ++iterationInd)
        {
            const double error = problem.computeError(currentVariables);
            if (std::abs(error - previousError) < epsilonHard && previousError != 0){
                if (verbose)
                {
                    qDebug() << "Stopped by low error change";
                }
                break;
            }
            previousError = error;

            const QVector<double> gradient = problem.computeGradient(currentVariables);
            const double gradNorm = vectorNorm(gradient);
            if (verbose)
            {
                qDebug()
                        << "Iteration:" << iterationInd + 1 << "/" << nMaxIterations
                        << "error:" << error
                        << "gradient norm" << gradNorm
                        << "current transform" << RigidAlignmentScalingProblem::transformationVectorFromVars(currentVariables);
            }

            if (callback != nullptr)
            {
                callback->call(currentVariables);
            }

            if (gradNorm < gradientNormThreshold)
            {
                if (verbose)
                    qDebug() << "Stopped by gradient norm";
                break;
            }
            const QVector<double> step = mulVectorByScalar(-stepLength, gradient);
            currentVariables = addVectors(currentVariables, step);
        }
        return currentVariables;
}
