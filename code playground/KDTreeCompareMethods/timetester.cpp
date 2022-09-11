#include "timetester.h"
using namespace KDTree;

void TimeTester::testBuildTreeFromFile01(QString fileName, QVector3D pointToSearch)
{
    ObjReadingTools::ObjFileData fileData;
    QString errorMessage;

    ObjReadingTools::readFile(fileName, fileData, errorMessage);

    QVector<QVector3D> points = fileData.getVertices();
    QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(fileData.getPolygonVertexIndices());
    QVector<int> polygonStart = MeshTools::buildPolygonStartVector(fileData.getPolygonVertexIndices());

    QVector<int> triangleVertexIndices = MeshTools::buildTriangleVertexIndices(polygonVertexIndices, polygonStart);

    QElapsedTimer timer;
    timer.start();

    KDTree::Node* tree = KDTree::buildBalancedTreeWithIndices(triangleVertexIndices, points, 2);
//    KDTree::Node* tree = KDTree::buildTreeWithIndices(triangleVertexIndices, points, 2);

    int pointIndex = tree->findNearestPointIndex(
                pointToSearch,
                points
                );

    const qint64 elapsed = timer.elapsed();

    delete tree;
    qDebug() << "\nFile:" << fileName <<  "\nTime elapsed: " << elapsed  << "\nPoint is " << points[pointIndex] << "\n";
}
