#ifndef REGISTRATION_STEPCALLBACK_H
#define REGISTRATION_STEPCALLBACK_H

#include <QVector>
#include <QVector3D>

namespace Registration {

class StepCallback
{
public:
    virtual void call(const QVector<QVector3D>& vertices) = 0;
};

}

#endif
