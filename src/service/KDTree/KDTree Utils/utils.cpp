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

QList<int> Utils::buildEnumeratedList(int nElements)
{
    QList<int> list;
    for (int elementInd = 0; elementInd < nElements; ++elementInd)
    {
        list << elementInd;
    }
    return list;
}
