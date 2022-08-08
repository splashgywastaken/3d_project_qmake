#include "rigidfitter.h"

#include <src/service/Optimization/GradientDescent/gradientdescent.h>
#include <src/service/Optimization/LambdaProblem/lambdaproblem.h>
#include <src/service/GeomTools/GeomTools.h>
#include <src/service/Optimization/LambdaStepCallback/lambdastepcallback.h>

const double rotationScale  = 10;
const double normalizationDiagonal = 2;

Registration::RigidFitter::RigidFitter(
        const QVector<QVector3D> &baseVertices,
        const double optimizationStepLength,
        const int optimizationNMaxSteps
        ) :
    m_baseVertices(baseVertices),
    m_optimizationStepLength(optimizationStepLength),
    m_optimizationNMaxSteps(optimizationNMaxSteps)
{
    Q_ASSERT(m_baseBboxDiagonal > 0);
    qDebug() << "m_baseBboxDiagonal " << m_baseBboxDiagonal;
}

QVector<QVector3D> Registration::RigidFitter::fit(
        const QVector<int> &verticesToFitIndices,
        const QVector<QVector3D> &verticesToFit,
        const double fitWeight,
        StepCallback *callback
        )
{
    const float rescaleConstant = normalizationDiagonal / m_baseBboxDiagonal;
    const auto errorFunction = [&](const QVector<double>& variables) -> double
    {
        const QMatrix4x4 transformation = transformationMatrixFromVariables(variables);

        const int nVerticesToFit = verticesToFitIndices.size();
        double resultError = 0;
        for (int loopIndex = 0; loopIndex < nVerticesToFit; loopIndex++)
        {
            const int index = verticesToFitIndices[loopIndex];
            const QVector3D newVertex = transformation.mapVector(m_baseVertices[index]);
            resultError += (newVertex * rescaleConstant - verticesToFit[loopIndex] * rescaleConstant).lengthSquared();
        }

        return fitWeight * resultError;
    };

    const auto gradientFunction = [&](const QVector<double>& variables) -> QVector<double>
    {
        return Optimization::numericalGradient(errorFunction, variables, 1e-3);
    };
    Optimization::LambdaProblem problem(errorFunction, gradientFunction);

    const auto optimizationCallbackFunction = [&](const QVector<double>& variables)
    {
        if (callback == nullptr)
        {
            return;
        }

        const QMatrix4x4 transformation = transformationMatrixFromVariables(variables);
        const QVector<QVector3D> vertices = GeomTools::transformVertices3D(
                        transformation, m_baseVertices
                    );
        callback->call(vertices);
    };
    Optimization::LambdaStepCallback optimizationCallback(optimizationCallbackFunction);

    const int nLineSearchIterations = 10;
    const double stepLengthMax = 10;
    m_variableValues = Optimization::gradientDescentWithBackTrackingLineSearch(
                problem, m_variableValues,
                m_optimizationStepLength, m_optimizationNMaxSteps, m_gradThreshold, nLineSearchIterations, stepLengthMax, m_verboseOptimization,
                &optimizationCallback, &m_optimizationStepLength
                );

    const QMatrix4x4 transformation = transformationMatrixFromVariables(m_variableValues);
    const QVector<QVector3D> vertices = GeomTools::transformVertices3D(
                transformation, m_baseVertices
                );
    return vertices;
}

QMatrix4x4 Registration::RigidFitter::transformationMatrixFromVariables(const QVector<double> &variables)
{
    const int nVariables = variables.size();
    Q_ASSERT(nVariables == 6);
    QMatrix4x4 transformation;

    const QVector3D translation(variables[0], variables[1], variables[2]);
    transformation.translate(translation);

    transformation.rotate(rotationScale * variables[3], QVector3D(1, 0, 0));
    transformation.rotate(rotationScale * variables[4], QVector3D(0, 1, 0));
    transformation.rotate(rotationScale * variables[5], QVector3D(0, 0, 1));

    return transformation;
}
