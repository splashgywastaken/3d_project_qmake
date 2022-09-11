#ifndef REGISTRATION_CLOSESTPOINTSFINDERKDTREE_H
#define REGISTRATION_CLOSESTPOINTSFINDERKDTREE_H

#include <src/service/Registration/ClosestPointsFinder/closestpointsfinder.h>
#include <src/service/KDTree/kdtree.h>

namespace Registration {

class ClosestPointsFinderKDTree : public ClosestPointsFinder
{
public:
    ClosestPointsFinderKDTree(const QVector<QVector3D>& vertices);

    QVector<int> findClosestPointsIndices(const QVector<QVector3D>& queryVertices) override;

private:
    QVector<QVector3D> m_vertices;
    std::unique_ptr<KDTree::Node> m_kdTree;
};

}

#endif
