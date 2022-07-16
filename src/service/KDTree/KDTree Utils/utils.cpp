#include "utils.h"
using namespace KDTree;

void Utils::findPointsRange(const QList<int> &pointIndices, const QVector<QVector3D> &points, QVector3D &min, QVector3D &max)
{
    QVector<float> minPoint = MAX_POINT;
    QVector<float> maxPoint = MIN_POINT;
    for (const int &pointInd : pointIndices)
    {
        for (int dim = 0; dim < 3; ++dim)
        {
            const float value = points[pointInd][dim];
            if (value < minPoint[dim])
            {
                minPoint[dim] = value;
            }
            if (value > maxPoint[dim])
            {
                maxPoint[dim] = value;
            }
        }
    }
    min = QVector3D(minPoint[0], minPoint[1], minPoint[2]);
    max = QVector3D(maxPoint[0], maxPoint[1], maxPoint[2]);
}

int Utils::findLargestComponentIndex(const QVector3D &v)
{
    if (v.x() >= qMax(v.y(),v.z()))
    {
        return 0;
    }
    if (v.y() >= qMax(v.x(),v.z()))
    {
        return 1;
    }
    if (v.z() >= qMax(v.x(),v.y()))
    {
        return 2;
    }
    return -1;
}

void Utils::splitPointsByPlane(
        const QList<int> &pointIndices, const QVector<QVector3D> &points,
        const int planeAxis, const float planePos,
        QList<int> &leftPointIndices, QList<int> &rightPointIndices
        )
{
    leftPointIndices.clear();
    rightPointIndices.clear();

    for (const int &pointInd : pointIndices)
    {
        if (points[pointInd][planeAxis] < planePos)
        {
            leftPointIndices << pointInd;
        }
        else
        {
            rightPointIndices << pointInd;
        }
    }
}

// Splits points by given values:
// pointIndices - indices to split;
// points - value of points to compare them with given indices;
// splitAxis - values from 0 to 2, represents which axis would be used in comparison;
// leftPointIndice and righPointIndices - Lists to split points into;
void Utils::splitPointsByPoint(QList<int> &pointIndices, QVector<QVector3D> points, QVector3D point, int splitAxis, QList<int> &leftPointIndices, QList<int> &rightPointIndices)
{
    leftPointIndices.clear();
    rightPointIndices.clear();

    for (const int& index : pointIndices)
    {
        if (points[index][splitAxis] < point[splitAxis])
        {
            leftPointIndices << index;
        }
        else
        {
            rightPointIndices << index;
        }
    }
}

QList<int> Utils::buildEnumeratedList(int nElements)
{
    QList<int> list;
    for (int elementInd = 0; elementInd < nElements; ++elementInd)
    {
        list << elementInd;
    }
    return list;
}

void Utils::buildSortedLists(QList<int> &pointIndices, QVector<QVector3D> &points, int axisToSort)
{
    if (pointIndices.count() <= 1)
    {
        return;
    }

    quickSort(pointIndices, points, axisToSort, 0, pointIndices.count() - 1);
}

void Utils::swap(int* lvalue, int* rvalue)
{
    int t = *lvalue;
    *lvalue = *rvalue;
    *rvalue = t;
}

int partition(QList<int>& indices, QVector<QVector3D>& points, int axisToSort, int p, int r)
{
    float x = points[indices[r]][axisToSort];
    int i = p - 1;

    for (int j = p; j <= r - 1; j++)
    {
        if (points[indices[j]][axisToSort] <= x)
        {
            i++;
            int temp = indices[j];
            indices[j] = indices[i];
            indices[i] = temp;
        }
    }
    Utils::swap(&indices[i + 1], &indices[r]);
    return (i + 1);
}

void Utils::quickSort(QList<int> &indices, QVector<QVector3D> &points, int axisToSort, int p, int r)
{
    if (p < r)
    {
        int q = partition(indices, points, axisToSort, p, r);
        quickSort(indices, points, axisToSort, p, q - 1);
        quickSort(indices, points, axisToSort, q + 1, r);
    }
}
