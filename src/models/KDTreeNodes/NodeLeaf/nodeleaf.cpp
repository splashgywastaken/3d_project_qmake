#include "nodeleaf.h"
#include "float.h"

KDTree::NodeLeaf::NodeLeaf(const QList<int> &pointIndices, const QVector<QVector3D> &points)
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

KDTree::NodeLeaf::NodeLeaf(const NodeLeaf &other) :
    Node(other),
    m_pointIndices(other.m_pointIndices),
    m_subsettedPoints(other.m_subsettedPoints)
{
}

void KDTree::NodeLeaf::findNearestPoint(const QVector3D &point, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
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

int KDTree::NodeLeaf::findNearestPointIndex(const QVector3D &point, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    int nearestPointInd = -1;
    float distSquared = FLT_MAX;
    findNearestPoint(point, points, nearestPointInd, distSquared);
    if (nearestPointDistSquared)
        *nearestPointDistSquared = distSquared;
    return nearestPointInd;
}

void KDTree::NodeLeaf::findNearestPoint(int pointInd, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const
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

int KDTree::NodeLeaf::findNearestPointIndex(int pointInd, const QVector<QVector3D> &points, float *nearestPointDistSquared) const
{
    int nearestPointInd = -1;
    float distSquared = FLT_MAX;
    findNearestPoint(pointInd, points, nearestPointInd, distSquared);
    if (nearestPointDistSquared)
        *nearestPointDistSquared = distSquared;
    return nearestPointInd;
}

int KDTree::NodeLeaf::memUsage() const
{
    return m_pointIndices.count() * static_cast<int>(sizeof(float)) + m_subsettedPoints.count() * static_cast<int>(sizeof(QVector3D));
}

KDTree::Node *KDTree::NodeLeaf::copy() const
{
    return new NodeLeaf(*this);
}
