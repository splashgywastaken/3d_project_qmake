#include "geomtoolstests.h"

#include <src/service/GeomTools/GeomTools.h>

const float epsilonWeak = 1e-3;
const float epsilonHard = 1e-7;

void GeomTools::Tests::GeomToolsTests::testBboxDiagonalLengthFromVertices01()
{
    const QVector<QVector3D> vertices = {QVector3D(-5, 4, -3)};
    const double res = GeomTools::bboxDiagonalLengthFromVertices(vertices);
    QVERIFY(std::abs(res - 0.0f) < epsilonHard);
}


void GeomTools::Tests::GeomToolsTests::testBboxDiagonalLengthFromVertices02()
{
    const QVector<QVector3D> vertices = {
        QVector3D(-5, 4, -3),
        QVector3D(-10, -10, -10),
        QVector3D(10, 10, 10),
    };
    const double res = GeomTools::bboxDiagonalLengthFromVertices(vertices);
    QVERIFY(std::abs(res - 34.64101615137755f) < epsilonWeak);
}

void GeomTools::Tests::GeomToolsTests::testBboxDiagonalLengthFromVertices03()
{
    const QVector<QVector3D> vertices = {
        QVector3D(10, 10, -10),
        QVector3D(-10, -10, 10),
    };
    const double res = GeomTools::bboxDiagonalLengthFromVertices(vertices);
    QVERIFY(std::abs(res - 34.64101615137755f) < epsilonWeak);
}

void GeomTools::Tests::GeomToolsTests::testBboxDiagonalLengthFromVertices04()
{
    const QVector<QVector3D> vertices = {
        QVector3D(-10, 5, 9),
        QVector3D(0, 5, 9),
    };
    const double res = GeomTools::bboxDiagonalLengthFromVertices(vertices);
    QVERIFY(std::abs(res - 10.0f) < epsilonHard);
}

void GeomTools::Tests::GeomToolsTests::testBboxDiagonalLengthFromVertices05()
{
    const QVector<QVector3D> vertices = {
        QVector3D(-10, 5, 9),
        QVector3D(0, 5, 9),
        QVector3D(-1, 5, 9),
    };
    const double res = GeomTools::bboxDiagonalLengthFromVertices(vertices);
    QVERIFY(std::abs(res - 10.0f) < epsilonHard);
}
