#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QVector3D>

namespace KDTree {

class Node
{
public:
    explicit Node() {};
    explicit Node(const Node& other);
    virtual ~Node() = default;

    virtual void findNearestPoint(const QVector3D &point, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const;
    virtual int findNearestPointIndex(const QVector3D &point, const QVector<QVector3D> &points, float *nearestPointDistSquared = nullptr) const;
    virtual void findNearestPoint(int pointInd, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const;
    virtual int findNearestPointIndex(int pointInd, const QVector<QVector3D> &points, float *nearestPointDistSquared = nullptr) const;

    virtual int memUsage() const;
    virtual Node *copy() const;
};

}

#endif // NODE_H
