#ifndef REGISTRATION_LAMBDASTEPCALLBACK_H
#define REGISTRATION_LAMBDASTEPCALLBACK_H

#include <src/service/Registration/FitterClasses/StepCallback/stepcallback.h>

namespace Registration {

class LambdaStepCallback : StepCallback
{
public:
    using Function = std::function<void(const QVector<QVector3D>&)>;
    LambdaStepCallback(Function function);

    void call(const QVector<QVector3D>& vertices) override;

private:
    Function m_function;
};

}

#endif
