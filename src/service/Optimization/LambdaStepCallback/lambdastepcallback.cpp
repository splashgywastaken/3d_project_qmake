#include "lambdastepcallback.h"

namespace Optimization {

LambdaStepCallback::LambdaStepCallback(Func func)
    : m_func(func)
{

}

void LambdaStepCallback::call(const QVector<double> &vars)
{
    m_func(vars);
}

}
