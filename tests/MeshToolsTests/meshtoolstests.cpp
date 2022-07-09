#include "meshtoolstests.h"
#include "src/service/MeshTools/meshtools.h"

using namespace MeshTools;

MeshToolsTests::MeshToolsTests(QObject *parent)
    : QObject{parent}
{
}

void MeshToolsTests::testBuildPolygonVertexIndicesVector01()
{
    QVector<QVector<int> > polygonVertexIndices = {
        { 0, 1, 4 },
        { 1, 2, 3, 4 }
    };
    QVector<int> expected = { 0, 1, 4, 1, 2, 3, 4 };
    QCOMPARE(buildPolygonVertexIndicesVector(polygonVertexIndices), expected);
}

void MeshToolsTests::testBuildPolygonVertexIndicesVector02()
{
    QCOMPARE(buildPolygonVertexIndicesVector(QVector<QVector<int> >()), QVector<int>());
}

void MeshToolsTests::testBuildPolygonStartVector01()
{
    QVector<QVector<int> > polygonVertexIndices = {
        { 0, 1, 4 },
        {},
        { 1, 2, 3, 4 }
    };
    QVector<int> expected = { 0, 3, 3, 7 };
    QCOMPARE(buildPolygonStartVector(polygonVertexIndices), expected);
}

void MeshToolsTests::testBuildPolygonStartVector02()
{
    QVector<int> expected = { 0 };
    QCOMPARE(buildPolygonStartVector(QVector<QVector<int> >()), expected);
}

void MeshToolsTests::testBuildTriangleVertexIndices01()
{
    QVector<int> polygonIndices = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    QVector<int> polygonStart = { 0, 3, 9 };
    QVector<int> triangleIndices = buildTriangleVertexIndices(polygonIndices, polygonStart);
    QVector<int> triangleIndicesExpected = { 0, 1, 2, 3, 4, 5, 3, 5, 6, 3, 6, 7, 3, 7, 8 };
    QCOMPARE(triangleIndices, triangleIndicesExpected);
}

void MeshToolsTests::testBuildTriangleVertexIndices02()
{
    QVector<int> polygonIndices = { 1, 0, 3, 2 };
    QVector<int> polygonStart = { 0, 4 };
    QVector<int> triangleIndices = buildTriangleVertexIndices(polygonIndices, polygonStart);
    QVector<int> triangleIndicesExpected = { 1, 0, 3, 1, 3, 2 };
    QCOMPARE(triangleIndices, triangleIndicesExpected);
}

void MeshToolsTests::testPackTriangleVertexCoords01()
{
    QVector<QVector3D> vertices = {
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 0, 0 },
        { 1, 1, 0 }
    };
    QVector<int> triangleVertexIndices = {
        0, 1, 3,
        1, 2, 3
    };
    QVector<float> expected = {
        0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        1, 0, 0,
        2, 0, 0,
        1, 1, 0
    };
    QCOMPARE(packTriangleVertexCoords(vertices, triangleVertexIndices), expected);
}