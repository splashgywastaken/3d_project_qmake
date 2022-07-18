#ifndef LAMBDASTEPCALLBACK_H
#define LAMBDASTEPCALLBACK_H

#include <src/service/Optimization/StepCallback/stepcallback.h>

namespace Optimization {

class LambdaStepCallback : public StepCallback
{
public:
    using Func = std::function<void (const QVector<double>&)>;
    LambdaStepCallback(Func func);
    void call(const QVector<double>& vars) override;

private:
    Func m_func;
};

}

#endif
