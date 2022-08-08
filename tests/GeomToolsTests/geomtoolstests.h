#ifndef GEOMTOOLS_TESTS_GEOMTOOLSTESTS_H
#define GEOMTOOLS_TESTS_GEOMTOOLSTESTS_H

#include <QTest>

namespace GeomTools {
namespace Tests {

class GeomToolsTests : public QObject
{
Q_OBJECT
private slots:
    void testBboxDiagonalLengthFromVertices01();
    void testBboxDiagonalLengthFromVertices02();
    void testBboxDiagonalLengthFromVertices03();
    void testBboxDiagonalLengthFromVertices04();
    void testBboxDiagonalLengthFromVertices05();
};

}
}

#endif
