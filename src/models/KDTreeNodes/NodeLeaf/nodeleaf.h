#ifndef NODELEAF_H
#define NODELEAF_H

#include "node.h"
#include <QList>

namespace KDTree {

class NodeLeaf : public Node
{
public:
    explicit NodeLeaf(const QList<int> &pointIndices, const QVector<QVector3D> &points);
    explicit NodeLeaf(const NodeLeaf& other);
    ~NodeLeaf() = default;

    void findNearestPoint(const QVector3D &point, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const override;
    int findNearestPointIndex(const QVector3D &point, const QVector<QVector3D> &points, float *nearestPointDistSquared = nullptr) const override;

    void findNearestPoint(int pointInd, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const override;
    int findNearestPointIndex(int pointInd, const QVector<QVector3D> &points, float *nearestPointDistSquared = nullptr) const override;

    void findNearestPointIndexInRadius(
            const QVector3D &point, const QVector<QVector3D> &points,
            int &nearestPointIndex, float &radius, float& nearestPointDistSquared
            ) const override;
    int findNearestPointIndexInRadius(
            const QVector3D &point, const QVector<QVector3D> &points,
            float &radius, float* nearestPointDist = nullptr
            ) const override;
    void findNearestPointInRadius(
            int pointInd, const QVector<QVector3D> &points,
            int &nearestPointIndex, float &radius, float &nearestPointDist
            ) const override;
    int findNearestPointIndexInRadius(
            int pointInd, const QVector<QVector3D> &points,
            float &radius, float *nearestPointDist = nullptr
            ) const override;

    int memUsage() const override;
    Node *copy() const override;

public:
    const QVector<int> m_pointIndices;
    QVector<QVector3D> m_subsettedPoints;
};

#endif // NODELEAF_H

}
