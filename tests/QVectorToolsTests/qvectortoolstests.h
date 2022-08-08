#ifndef QVECTORTOOLS_TESTS_QVECTORTOOLSTESTS_H
#define QVECTORTOOLS_TESTS_QVECTORTOOLSTESTS_H

#include <QTest>

namespace QVectorTools {
namespace Tests {

class QVectorToolsTests : public QObject
{
Q_OBJECT
private slots:
    void testSubset01();
    void testSubset02();
    void testSubset03();
    void testRange01();
    void testRange02();
    void testRange03();
};

}
}
#endif
