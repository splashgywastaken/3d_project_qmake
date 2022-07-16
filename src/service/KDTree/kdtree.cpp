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

KDTree::Node *KDTree::buildBalancedTreeWithIndices(
        QList<int>& pointIndices, QVector<QVector3D>& points,
        int nPointsInLeaf,  int sortAxis
        )
{
    // Algorythm
    // 1. Build sorted QList with indices and points
    // 2. Get median
    // 3. Split list in two lists
    // 4. create a childLeft element with left array and childRight element with right array
    // 5. Create and return new inner node
    if (pointIndices.count() <= nPointsInLeaf)
    {
        return new KDTree::NodeLeaf(pointIndices, points);
    }

    // 1. Build sorted QList with indices and points
    // Firstly sorts by x, then by y and then by z, repeats process
    if (sortAxis >= 3)
    {
        sortAxis = 0;
    }
    Utils::buildSortedLists(pointIndices, points, sortAxis);

    // 2. Get median
    const QVector3D medianPoint = points[pointIndices[pointIndices.count() / 2.0f]];
    const float splitPos = medianPoint[sortAxis];
    QList<int> leftPointIndices;
    QList<int> rightPointIndices;

    // 3. Split list in two lists
    KDTree::Utils::splitPointsByPoint(pointIndices, points, medianPoint, sortAxis, leftPointIndices, rightPointIndices);
    if (leftPointIndices.isEmpty())
        return new NodeLeaf(pointIndices, points);

    // 4. create a childLeft element with left array and childRight element with right array
    const KDTree::Node *childLeft = KDTree::buildBalancedTreeWithIndices(leftPointIndices,points,nPointsInLeaf, sortAxis + 1);
    const KDTree::Node *childRight = KDTree::buildBalancedTreeWithIndices(rightPointIndices,points,nPointsInLeaf, sortAxis + 1);
    // 5. Create and return new inner node
    return new NodeInner(childLeft,childRight,sortAxis,splitPos);
}

KDTree::Node *KDTree::buildBalancedTree(QVector<QVector3D> &points, int nPointsInLeaf)
{
    QList<int> pointIndices = KDTree::Utils::buildEnumeratedList(points.count());
    return KDTree::buildBalancedTreeWithIndices(pointIndices, points, nPointsInLeaf, false);
}
