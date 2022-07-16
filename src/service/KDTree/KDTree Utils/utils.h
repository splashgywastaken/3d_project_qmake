#ifndef KDTREE_UTILS_H
#define KDTREE_UTILS_H

#include "float.h"
#include <QList>
#include <QVector>
#include <QVector3D>
#include <node.h>

namespace KDTree {

namespace Utils {

void findPointsRange(const QList<int>& pointIndices, const QVector<QVector3D>& points, QVector3D& min, QVector3D& max);
int findLargestComponentIndex(const QVector3D& vector);
// swaps two values between them
void swap(int* lvalue, int* rvalue);

void splitPointsByPlane(
        const QList<int>& pointIndices, const QVector<QVector3D>& points,
        const int planeAxis, const float planePos,
        QList<int>& leftPointIndices, QList<int> &rightPointIndices
        );
void splitPointsByPoint(
        QList<int> &pointIndices, QVector<QVector3D> points,
        QVector3D point, int splitAxis,
        QList<int>& leftPointIndices, QList<int> &rightPointIndices
        );

QList<int> buildEnumeratedList(int nElements);

void buildSortedLists(QList<int>& pointIndices, QVector<QVector3D>& points, int axisToSort);

void quickSort(QList<int>& indices, QVector<QVector3D>& points, int axisToSort, int p, int r);

// Array sorting helper functions:
int findSmallestIndex(QList<int> indices, QVector<QVector3D>& points, int index);

const QVector<float> MAX_POINT = QVector<float>() << FLT_MAX << FLT_MAX << FLT_MAX;
const QVector<float> MIN_POINT = QVector<float>() << -FLT_MAX << -FLT_MAX << -FLT_MAX;

}
}

#endif
