#ifndef KDTREE_UTILS_H
#define KDTREE_UTILS_H

#include "float.h"
#include <QList>
#include <QVector>
#include <QVector3D>

namespace KDTree {

namespace Utils {

void findPointsRange(const QList<int>& pointIndices, const QVector<QVector3D>& points, QVector3D& min, QVector3D& max);
int findLargestComponentIndex(const QVector3D& vector);

void splitPointsByPlane(
        const QList<int>& pointIndices, const QVector<QVector3D>& points,
        const int planeAxis, const float planePos,
        QList<int>& leftPointIndices, QList<int> &rightPointIndices
        );

QList<int> buildEnumeratedList(int nElements);

const QVector<float> MAX_POINT = QVector<float>() << FLT_MAX << FLT_MAX << FLT_MAX;
const QVector<float> MIN_POINT = QVector<float>() << -FLT_MAX << -FLT_MAX << -FLT_MAX;

}
}

#endif
