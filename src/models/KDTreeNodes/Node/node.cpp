#include "node.h"

#include <QDebug>

using namespace KDTree;

Node::Node(const Node &other)
{
    Q_UNUSED(other);
}

void Node::findNearestPoint(const QVector3D &point, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
{
    qDebug() << "Calling virtual method Node::findNearestPoint";
    Q_ASSERT(false);
    Q_UNUSED(point)
    Q_UNUSED(points)
    Q_UNUSED(nearestPointInd)
    Q_UNUSED(nearestPointDistSquared)
}

int Node::findNearestPointIndex(const QVector3D &point, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    qDebug() << "Calling virtual method Node::findNearestPointInd";
    Q_ASSERT(false);
    Q_UNUSED(point)
    Q_UNUSED(points)
    Q_UNUSED(nearestPointDistSquared)
    return -1;
}

void Node::findNearestPoint(int pointInd, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
{
    qDebug() << "Calling virtual method Node::findNearestPoint";
    Q_ASSERT(false);
    Q_UNUSED(pointInd)
    Q_UNUSED(points)
    Q_UNUSED(nearestPointInd)
    Q_UNUSED(nearestPointDistSquared)
}

int Node::findNearestPointIndex(int pointInd, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    qDebug() << "Calling virtual method Node::findNearestPointInd";
    Q_ASSERT(false);
    Q_UNUSED(pointInd)
    Q_UNUSED(points)
    Q_UNUSED(nearestPointDistSquared)
    return -1;
}

void Node::findNearestPointIndexInRadius(
        const QVector3D& point, const QVector<QVector3D> &points,
        int &nearestPointIndex, float& radius, float& nearestPointDistSquared
        ) const
{
    qDebug() << "Calling virtual method Node::findNearestPointInRadius";
    Q_ASSERT(false);
    Q_UNUSED(point);
    Q_UNUSED(points);
    Q_UNUSED(nearestPointIndex);
    Q_UNUSED(nearestPointDistSquared);
    Q_UNUSED(radius);
}

int Node::findNearestPointIndexInRadius(const QVector3D &point, const QVector<QVector3D> &points, float &radius, float* nearestPointDist) const
{
    qDebug() << "Calling virtual method Node::findNearestPointIndexInRadius";
    Q_ASSERT(false);
    Q_UNUSED(point);
    Q_UNUSED(points);
    Q_UNUSED(radius);
    Q_UNUSED(nearestPointDist);
    return -1;
}

int Node::findNearestPointIndexInRadius(
        int pointInd, const QVector<QVector3D>& points,
        float& radius, float* nearestPointDist
        ) const
{
    qDebug() << "Calling virtual method Node::findNearestPointInRadius";
    Q_ASSERT(false);
    Q_UNUSED(pointInd);
    Q_UNUSED(points);
    Q_UNUSED(radius);
    Q_UNUSED(nearestPointDist);
    return -1;
}

void Node::findNearestPointInRadius(
        int pointInd, const QVector<QVector3D> &points,
        int &nearestPointIndex, float &radius, float &nearestPointDist
        ) const
{
    qDebug() << "Calling virtual method Node::findNearestPointInRadius";
    Q_ASSERT(false);
    Q_UNUSED(pointInd);
    Q_UNUSED(points);
    Q_UNUSED(nearestPointIndex);
    Q_UNUSED(radius);
    Q_UNUSED(nearestPointDist);
}

int Node::memUsage() const
{
    Q_ASSERT_X(false, "Calling virtual method memUsage()", "");
    return -1;
}

Node* Node::copy() const
{
    Q_ASSERT_X(false, "Node::copy()", "Calling virtual method");
    return nullptr;
}
