#include "kdtree.h"
#include "utils.h"

KDTree::Node *KDTree::buildTreeWithIndices(const QList<int> &pointIndices, const QVector<QVector3D> &points, int nPointsInLeaf)
{
    if (pointIndices.count() <= nPointsInLeaf)
        return new KDTree::NodeLeaf(pointIndices, points);

    QVector3D minPoint;
    QVector3D maxPoint;
    KDTree::Utils::findPointsRange(pointIndices,points,minPoint,maxPoint);
    const QVector3D sizes = maxPoint - minPoint;
    const int splitAxis = KDTree::Utils::findLargestComponentIndex(sizes);
    const float splitPos = minPoint[splitAxis] + sizes[splitAxis] / 2.0f;

    QList<int> leftPointIndices;
    QList<int> rightPointIndices;
    KDTree::Utils::splitPointsByPlane(pointIndices,points,splitAxis,splitPos,leftPointIndices,rightPointIndices);
    if (leftPointIndices.isEmpty())
        return new NodeLeaf(pointIndices, points);

    const KDTree::Node *childLeft = KDTree::buildTreeWithIndices(leftPointIndices,points,nPointsInLeaf);
    const KDTree::Node *childRight = KDTree::buildTreeWithIndices(rightPointIndices,points,nPointsInLeaf);
    return new NodeInner(childLeft,childRight,splitAxis,splitPos);
}

KDTree::Node *KDTree::buildTree(const QVector<QVector3D> &points, int nPointsInLeaf)
{
    QList<int> pointIndices = KDTree::Utils::buildEnumeratedList(points.count());
    return KDTree::buildTreeWithIndices(pointIndices,points,nPointsInLeaf);
}
