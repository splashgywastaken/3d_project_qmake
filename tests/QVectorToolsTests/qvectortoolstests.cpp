#include "qvectortoolstests.h"

#include <src/service/QVectorTools/QVectorTools.h>

void QVectorTools::Tests::QVectorToolsTests::testSubset01()
{
    const QVector<char> vec = {};
    const QVector<int> indices = {};
    const QVector<char> res = QVectorTools::subset(indices, vec);
    const QVector<char> ans = {};
    QCOMPARE(ans, res);
}


void QVectorTools::Tests::QVectorToolsTests::testSubset02()
{
    const QVector<char> vec = {'1', '2', '9'};
    const QVector<int> indices = {};
    const QVector<char> res = QVectorTools::subset(indices, vec);
    const QVector<char> ans = {};
    QCOMPARE(ans, res);
}


void QVectorTools::Tests::QVectorToolsTests::testSubset03()
{
    const QVector<char> vec = {'1', '2', '9'};
    const QVector<int> indices = {2, 2, 0};
    const QVector<char> res = QVectorTools::subset(indices, vec);
    const QVector<char> ans = {'9', '9', '1'};
    QCOMPARE(ans, res);
}

void QVectorTools::Tests::QVectorToolsTests::testRange01()
{
    const QVector<int> res = QVectorTools::range(0);
    const QVector<int> ans = {};
    QCOMPARE(ans, res);
}

void QVectorTools::Tests::QVectorToolsTests::testRange02()
{
    const QVector<int> res = QVectorTools::range(1);
    const QVector<int> ans = {0};
    QCOMPARE(ans, res);
}

void QVectorTools::Tests::QVectorToolsTests::testRange03()
{
    const QVector<int> res = QVectorTools::range(5);
    const QVector<int> ans = {0, 1, 2, 3, 4};
    QCOMPARE(ans, res);
}
