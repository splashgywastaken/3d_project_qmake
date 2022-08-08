#ifndef GEOMTOOLS_H
#define GEOMTOOLS_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

namespace GeomTools {

QVector<QVector3D> transformVertices3D(
        const QMatrix4x4 &transformation,
        const QVector<QVector3D> &vertices
        );

QVector<QVector3D> computeNormals(
        const QVector<int> &triangleVertexIndices,
        const QVector<QVector3D> &vertices
        );

float bboxDiagonalLengthFromVertices(const QVector<QVector3D> &vertices);

}
#endif
