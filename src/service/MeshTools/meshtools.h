#ifndef MESHTOOLS_H
#define MESHTOOLS_H

#include <QVector>
#include <QVector3D>

namespace MeshTools
{
    QVector<int> buildPolygonVertexIndicesVector(QVector<QVector<int> > polygonVertexIndices);
    QVector<int> buildPolygonStartVector(QVector<QVector<int> > polygonVertexIndices);
    QVector<int> buildTriangleVertexIndices(QVector<int> polygonIndices, QVector<int> polygonStart);
    QVector<float> packTriangleVertexCoords(QVector<QVector3D> vertices, QVector<int> triangleVertexIndices);

    float getMax(QVector<float> vector);
    float getMin(QVector<float> vector);

    QVector3D getMax(QVector<QVector3D> vector, bool getX, bool getY, bool getZ);
    QVector3D getMin(QVector<QVector3D> vector, bool getX, bool getY, bool getZ);
};

#endif // MESHTOOLS_H
