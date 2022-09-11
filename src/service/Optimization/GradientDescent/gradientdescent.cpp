#include "src/service/Optimization/GradientDescent/gradientdescent.h"

#include <QDebug>
#include <rigidalignmentscalingproblem.h>
#include "rigidalignmentproblem.h"

const double epsilonHard = 1e-9;
double previousError = 0;

QVector<double> Optimization::gradientDescent(
        Optimization::Problem *problem,
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
            const double error = problem->computeError(currentVariables);
            if (std::abs(error - previousError) < epsilonHard && previousError != 0){
                if (verbose)
                {
                    qDebug() << "Stopped by low error change";
                }
                break;
            }
            previousError = error;

            const QVector<double> gradient = problem->computeGradient(currentVariables);
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

QVector<double> Optimization::gradientDescentWithBackTrackingLineSearch(
        Problem &problem,
        const QVector<double>
        &initialVars,
        const double stepLengthInitial,
        const int nMaxIterations,
        const double gradientNormThreshold,
        const int nLineSearchIteration,
        const double stepLengthMax,
        const bool verbose,
        StepCallback *callback,
        double *outComputedStepLength
        )
{
    double stepLength = stepLengthInitial;

    // TODO: move it to params/settings
    const double stepDownscaleConstant = 0.7;
    const double stepUpscaleConstant = 1.2;

    QVector<double> currentVariables = initialVars;
    for (int iterationInd = 0; iterationInd < nMaxIterations; ++iterationInd)
    {
        const double error = problem.computeError(currentVariables);
        const QVector<double> gradient = problem.computeGradient(currentVariables);
        const double gradNorm = vectorNorm(gradient);
        if (verbose)
        {
            qDebug()
                    << "Iteration:" << iterationInd + 1 << "/" << nMaxIterations
                    << "error:" << error
                    << "gradient norm" << gradNorm
                    << "stepLength" << stepLength;
        }

        if (callback != nullptr)
        {
            callback->call(currentVariables);
        }

        if (gradNorm < gradientNormThreshold)
        {
            if (verbose)
            {
                qDebug() << "Stopped by gradient norm";
            }
            break;
        }

        for (int lineSearchIter = 0; lineSearchIter < nLineSearchIteration; ++lineSearchIter)
        {
            QVector<double> lineSearchVars = addVectors(currentVariables, mulVectorByScalar(-stepLength, gradient));
            const double newError = problem.computeError(lineSearchVars);
            qDebug() << "Line search" << lineSearchIter << error << newError << stepLength;
            if (newError < error)
            {
                if (lineSearchIter == 0)
                {
                    stepLength *= stepUpscaleConstant;
                    stepLength = std::min(stepLength, stepLengthMax);
                }
                break;
            }
            stepLength *= stepDownscaleConstant;
        }

        const QVector<double> step = mulVectorByScalar(-stepLength, gradient);
        currentVariables = addVectors(currentVariables, step);
    }

    if(outComputedStepLength != nullptr)
        *outComputedStepLength = stepLength;

    return currentVariables;
}
