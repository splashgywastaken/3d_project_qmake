#include "closestpointsfinderkdtree.h"

const int nPointsInLeaf = 16;

Registration::ClosestPointsFinderKDTree::ClosestPointsFinderKDTree(const QVector<QVector3D> &vertices)
{
    KDTree::Node *kdTree = KDTree::buildTree(vertices, nPointsInLeaf);
    m_kdTree = std::unique_ptr<KDTree::Node>(kdTree);
}

QVector<int> Registration::ClosestPointsFinderKDTree::findClosestPointsIndices(const QVector<QVector3D> &queryVertices)
{
    QVector<int> result;
    for (const QVector3D query : queryVertices)
    {
        const int index = m_kdTree->findNearestPointIndex(query, m_vertices);
        result << index;
    }

    return result;
}
