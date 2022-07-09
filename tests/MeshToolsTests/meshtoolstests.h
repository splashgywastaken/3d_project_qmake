#ifndef MESHTOOLSTESTS_H
#define MESHTOOLSTESTS_H

#include <QObject>
#include <QTest>

class MeshToolsTests : public QObject
{
    Q_OBJECT
public:
    explicit MeshToolsTests(QObject *parent = nullptr);
private slots:
    void testBuildPolygonVertexIndicesVector01();
    void testBuildPolygonVertexIndicesVector02();
    void testBuildPolygonStartVector01();
    void testBuildPolygonStartVector02();
    void testBuildTriangleVertexIndices01();
    void testBuildTriangleVertexIndices02();
    void testPackTriangleVertexCoords01();

    void testGetMin01();
    void testGetMin02();
    void testGetMin03();
    void testGetMin04();
    void testGetMin05();
    void testGetMax01();
    void testGetMax02();
    void testGetMax03();
    void testGetMax04();
    void testGetMax05();
};

#endif // MESHTOOLSTESTS_H
