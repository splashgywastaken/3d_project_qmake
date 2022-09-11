#ifndef REGISTRATION_FITTER_H
#define REGISTRATION_FITTER_H

#include <QVector>
#include <QVector3D>
#include <src/service/Registration/FitterClasses/StepCallback/stepcallback.h>

namespace Registration {

class Fitter
{
public:
    virtual ~Fitter() = default;
    virtual QVector<QVector3D> fit(
            const QVector<int>& verticesToFitIndices,
            const QVector<QVector3D>& verticesToFit,
            const double fitWieght,
            StepCallback* callback = nullptr
            ) = 0;

    virtual QVector<double> getTransformationVariablesVector() = 0;
};

}

#endif
