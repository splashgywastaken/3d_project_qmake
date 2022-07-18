#ifndef STEPCALLBACK_H
#define STEPCALLBACK_H

#include <QVector>

namespace Optimization {

class StepCallback
{
public:
    virtual void call(const QVector<double> &vars) = 0;
};

}

#endif
