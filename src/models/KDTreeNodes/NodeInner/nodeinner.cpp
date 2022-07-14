#include "nodeinner.h"
#include "float.h"
using namespace KDTree;

NodeInner::NodeInner(const Node *childLeft, const Node *childRight, int splitAxis, float splitPos) :
    KDTree::Node::Node(),
    m_childLeft(childLeft),
    m_childRight(childRight),
    m_splitAxis(splitAxis),
    m_splitPos(splitPos)
{}

NodeInner::NodeInner(const NodeInner &other) :
    Node(other),
    m_childLeft(other.m_childLeft->copy()),
    m_childRight(other.m_childRight->copy()),
    m_splitAxis(other.m_splitAxis),
    m_splitPos(other.m_splitPos)
{}

NodeInner::~NodeInner()
{
    delete m_childLeft;
    m_childLeft = nullptr;
    delete m_childRight;
    m_childRight = nullptr;
}

void NodeInner::findNearestPoint(const QVector3D &point, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
{
    const Child childToSearch = point[m_splitAxis] < m_splitPos ? Child_Left : Child_Right;
    child(childToSearch)->findNearestPoint(point, points, nearestPointInd, nearestPointDistSquared);

    const float distToSplitPlaneSquared = float(pow(m_splitPos - point[m_splitAxis], 2));
    if (distToSplitPlaneSquared > nearestPointDistSquared)
        return;
    const Child otherChild = static_cast<Child>((childToSearch + 1) % 2);
    child(otherChild)->findNearestPoint(point, points, nearestPointInd, nearestPointDistSquared);
}

int NodeInner::findNearestPointIndex(const QVector3D &point, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    int nearestPointInd = -1;
    float distSquared = FLT_MAX;
    findNearestPoint(point,points,nearestPointInd,distSquared);
    if (nearestPointDistSquared)
        *nearestPointDistSquared = distSquared;
    return nearestPointInd;
}

void NodeInner::findNearestPoint(int pointInd, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
{
    const QVector3D &point = points[pointInd];

    const Child childToSearch = point[m_splitAxis] < m_splitPos ? Child_Left : Child_Right;
    child(childToSearch)->findNearestPoint(pointInd, points, nearestPointInd, nearestPointDistSquared);

    if (nearestPointInd != -1) {
        const float distToSplitPlaneSquared = float(pow(m_splitPos - point[m_splitAxis], 2));
        if (distToSplitPlaneSquared > nearestPointDistSquared)
            return;
    }
    const Child otherChild = static_cast<Child>((childToSearch + 1) % 2);
    child(otherChild)->findNearestPoint(pointInd, points, nearestPointInd, nearestPointDistSquared);
}

int NodeInner::findNearestPointIndex(int pointInd, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    int nearestPointInd = -1;
    float distSquared = FLT_MAX;
    findNearestPoint(pointInd, points, nearestPointInd, distSquared);
    if (nearestPointDistSquared)
        *nearestPointDistSquared = distSquared;
    return nearestPointInd;
}

int NodeInner::memUsage() const
{
    return m_childLeft->memUsage() + m_childRight->memUsage();
}

Node *NodeInner::copy() const
{
    return new NodeInner(*this);
}

const Node *KDTree::NodeInner::child(Child child) const
{
    if (child == Child_Left)
    {
        return m_childLeft;
    }
    if (child == Child_Right)
    {
        return m_childRight;
    }

    Q_ASSERT(false);
    return nullptr;
}

