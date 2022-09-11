#ifndef REGISTRATION_CLOSESTPOINTSFINDER_H
#define REGISTRATION_CLOSESTPOINTSFINDER_H

#include <QVector>
#include <QVector3D>

namespace Registration {

class ClosestPointsFinder
{
public:
    virtual ~ClosestPointsFinder() = default;
    virtual QVector<int> findClosestPointsIndices(const QVector<QVector3D>& queryVertices) = 0;
};

}

#endif
