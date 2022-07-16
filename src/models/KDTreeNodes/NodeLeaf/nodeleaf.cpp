#include "nodeleaf.h"
#include "float.h"
using namespace KDTree;

NodeLeaf::NodeLeaf(const QList<int> &pointIndices, const QVector<QVector3D> &points)
    : Node(),
      m_pointIndices(pointIndices.toVector())
{
    const int nSubsettedPoints = pointIndices.size();
    m_subsettedPoints.resize(nSubsettedPoints);
    for (int pointIndex = 0; pointIndex < nSubsettedPoints; ++pointIndex)
    {
        m_subsettedPoints[pointIndex] = points[pointIndices[pointIndex]];
    }
}

NodeLeaf::NodeLeaf(const NodeLeaf &other) :
    Node(other),
    m_pointIndices(other.m_pointIndices),
    m_subsettedPoints(other.m_subsettedPoints)
{
}

void NodeLeaf::findNearestPoint(const QVector3D &point, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
{
    Q_UNUSED(points)
    const int nSubsettedPoints = m_pointIndices.size();
    for (int pointInd = 0; pointInd < nSubsettedPoints; ++pointInd) {
        const float distSquared = (m_subsettedPoints[pointInd] - point).lengthSquared();
        if (distSquared < nearestPointDistSquared) {
            nearestPointDistSquared = distSquared;
            nearestPointInd = m_pointIndices[pointInd];
        }
    }
}

int NodeLeaf::findNearestPointIndex(const QVector3D &point, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    int nearestPointInd = -1;
    float distSquared = FLT_MAX;
    findNearestPoint(point, points, nearestPointInd, distSquared);
    if (nearestPointDistSquared)
        *nearestPointDistSquared = distSquared;
    return nearestPointInd;
}

void NodeLeaf::findNearestPoint(int pointInd, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
{
    const QVector3D &point = points[pointInd];
    const int nSubsettedPoints = m_pointIndices.size();
    for (int otherPointInd = 0; otherPointInd < nSubsettedPoints; ++otherPointInd) {
        if (m_pointIndices[otherPointInd] == pointInd)
            continue;

        const float distSquared = (m_subsettedPoints[otherPointInd] - point).lengthSquared();
        if (distSquared < nearestPointDistSquared) {
            nearestPointDistSquared = distSquared;
            nearestPointInd = m_pointIndices[otherPointInd];
        }
    }
}

int NodeLeaf::findNearestPointIndex(int pointInd, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    int nearestPointInd = -1;
    float distSquared = FLT_MAX;
    findNearestPoint(pointInd, points, nearestPointInd, distSquared);
    if (nearestPointDistSquared)
        *nearestPointDistSquared = distSquared;
    return nearestPointInd;
}

void NodeLeaf::findNearestPointIndexInRadius(
        const QVector3D &point, const QVector<QVector3D> &points,
        int &nearestPointIndex, float &radius, float& nearestPointDistSquared
        ) const
{
    Q_UNUSED(points);
    const int nSubsettedPoints = m_pointIndices.size();
    for (int pointIndex = 0; pointIndex < nSubsettedPoints; pointIndex++)
    {
        const float distSquared = sqrt((m_subsettedPoints[pointIndex] - point).lengthSquared());
        if (distSquared < nearestPointDistSquared && distSquared <= pow(radius, 2))
        {
            nearestPointDistSquared = distSquared;
            nearestPointIndex = m_pointIndices[pointIndex];
        }
    }
}

int NodeLeaf::findNearestPointIndexInRadius(
        const QVector3D &point, const QVector<QVector3D> &points,
        float &radius, float* nearestPointDist
        ) const
{
    int nearestPointIndex = -1;
    if (radius <= 0.0f)
    {
        return nearestPointIndex;
    }
    float dist = FLT_MAX;
    findNearestPointIndexInRadius(point, points, nearestPointIndex, radius, dist);
    if (nearestPointDist != nullptr)
    {
        *nearestPointDist = dist;
    }
    return nearestPointIndex;
}

int NodeLeaf::findNearestPointIndexInRadius(
        int pointInd, const QVector<QVector3D> &points,
        float &radius, float *nearestPointDist
        ) const
{
    int nearestPointIndex = -1;
    if (radius <= 0.0f)
    {
        return nearestPointIndex;
    }
    float dist = FLT_MAX;
    findNearestPointInRadius(pointInd, points, nearestPointIndex, radius, dist);
    if (nearestPointDist)
    {
        *nearestPointDist = dist;
    }
    return nearestPointIndex;
}

void NodeLeaf::findNearestPointInRadius(
        int pointIndex, const QVector<QVector3D> &points,
        int &nearestPointIndex, float &radius, float &nearestPointDistSquared
        ) const
{
    if (radius <= 0.0f)
    {
        nearestPointIndex = -1;
        return;
    }
    const QVector3D &point = points[pointIndex];
    const int nSubsettedPoints = m_pointIndices.size();
    for (int otherPointIndex = 0; otherPointIndex < nSubsettedPoints; otherPointIndex++)
    {
        if (m_pointIndices[otherPointIndex] == pointIndex)
        {
            continue;
        }

        const float distSquared = (m_subsettedPoints[otherPointIndex] - point).lengthSquared();
        if (distSquared < nearestPointDistSquared && distSquared <= pow(radius, 2))
        {
            nearestPointDistSquared = distSquared;
            nearestPointIndex = m_pointIndices[otherPointIndex];
        }
    }
}

int KDTree::NodeLeaf::memUsage() const
{
    return m_pointIndices.count() * static_cast<int>(sizeof(float)) + m_subsettedPoints.count() * static_cast<int>(sizeof(QVector3D));
}

Node *KDTree::NodeLeaf::copy() const
{
    return new NodeLeaf(*this);
}
