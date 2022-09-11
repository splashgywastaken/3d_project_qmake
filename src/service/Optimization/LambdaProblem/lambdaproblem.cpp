#include "lambdaproblem.h"

Optimization::LambdaProblem::LambdaProblem(
        ErrorFunction errorFunction,
        GradientFunction gradientFunction
        ) :
    m_errorFunction(errorFunction),
    m_gradientFunction(gradientFunction)
{

}

double Optimization::LambdaProblem::computeError(const QVector<double> &variables)
{
    return m_errorFunction(variables);
}

QVector<double> Optimization::LambdaProblem::computeGradient(const QVector<double> &variables)
{
    return m_gradientFunction(variables);
}
