#include "lambdastepcallback.h"

Registration::LambdaStepCallback::LambdaStepCallback(Function function)
    : m_function(function)
{

}

void Registration::LambdaStepCallback::call(const QVector<QVector3D> &vertices)
{
    m_function(vertices);
}
