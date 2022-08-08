#include "GeomTools.h"

const int nVerticesInTriangle = 3;

QVector<QVector3D> GeomTools::transformVertices3D(const QMatrix4x4 &transformation, const QVector<QVector3D> &vertices)
{
    QVector<QVector3D> result;
    for (const auto vertex : vertices)
    {
        result << transformation.mapVector(vertex);
    }

    return result;
}

QVector<QVector3D> GeomTools::computeNormals(const QVector<int> &triangleVertexIndices, const QVector<QVector3D> &vertices)
{
    QVector<QVector3D> res(vertices.size());
   for(int triangleInd = 0; triangleInd < triangleVertexIndices.size() / 3; ++triangleInd){
       for(int vertexInd = 0; vertexInd < nVerticesInTriangle; ++vertexInd){
           const int prevVertexInd = triangleVertexIndices[3 * triangleInd + vertexInd % nVerticesInTriangle];
           const int originVertexInd = triangleVertexIndices[3 * triangleInd + (vertexInd + 1) % nVerticesInTriangle];
           const int nextVertexInd = triangleVertexIndices[3 * triangleInd + (vertexInd + 2) % nVerticesInTriangle];

           const QVector3D edge1 = vertices[prevVertexInd] - vertices[originVertexInd];
           const QVector3D edge2 = vertices[nextVertexInd] - vertices[originVertexInd];
           const QVector3D normal = QVector3D::crossProduct(edge2 * 1e3, edge1 * 1e3).normalized();

           res[triangleVertexIndices[3 * triangleInd + (vertexInd + 1) % nVerticesInTriangle]] += normal;
       }
   }
   for(int vertexInd = 0; vertexInd < res.size(); ++vertexInd)
       res[vertexInd].normalize();
   return res;
}

float GeomTools::bboxDiagonalLengthFromVertices(const QVector<QVector3D> &vertices)
{
    const int nVertices = vertices.size();
    Q_ASSERT(nVertices > 0);
    QVector3D minCorner(
                std::numeric_limits<float>::infinity(),
                std::numeric_limits<float>::infinity(),
                std::numeric_limits<float>::infinity());

    QVector3D maxCorner(
                -std::numeric_limits<float>::infinity(),
                -std::numeric_limits<float>::infinity(),
                -std::numeric_limits<float>::infinity());

    const int n3DDims = 3;
    for(int vertexInd = 0; vertexInd < nVertices; ++vertexInd){
        for(int dimInd = 0; dimInd < n3DDims; ++dimInd){
            const float value = vertices[vertexInd][dimInd];
            if(value <= minCorner[dimInd])
                minCorner[dimInd] = value;
            if(value >= maxCorner[dimInd])
                maxCorner[dimInd] = value;
        }
    }
    return (maxCorner - minCorner).length();
}
