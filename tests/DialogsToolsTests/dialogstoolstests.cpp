#include "dialogstoolstests.h"

#include <src/service/DialogsTools/dialogstools.h>

const float epsilonWeak = 1e-3;
const float epsilonHard = 1e-7;

namespace DialogsTools {
namespace Tests
{

void DialogsToolsTests::stringtoDoubleVectorTest01()
{
    const QString string = "";
    QVector<double> vector;

    QVERIFY(!stringToDoubleVector(string, vector));
}

void DialogsToolsTests::stringtoDoubleVectorTest02()
{
    const QString string = "0,0,0,0,0";
    QVector<double> vector;
    QVector<double> expected = {0,0,0,0,0};

    QVERIFY(stringToDoubleVector(string, vector));
    QCOMPARE(vector, expected);
}

void DialogsToolsTests::stringtoDoubleVectorTest03()
{
    const QString string = "0,0,0,a";
    QVector<double> vector;

    QVERIFY(!stringToDoubleVector(string, vector));
}

void DialogsToolsTests::stringtoDoubleVectorTest04()
{
    const QString string = "";
    QVector<double> vector;

    QVERIFY(!stringToDoubleVector(string, vector));
}

void DialogsToolsTests::stringtoDoubleVectorTest05()
{
    const QString string = "9e-2,11e-7,5e-10";
    QVector<double> vector;
    QVector<double> expected = {9e-2, 11e-7, 5e-10};

    QVERIFY(stringToDoubleVector(string, vector));
    QCOMPARE(vector, expected);
}

void DialogsToolsTests::parseDouble01()
{
    const QString string = "";
    double value;

    QVERIFY(!parseDouble(string, value));
}

void DialogsToolsTests::parseDouble02()
{
    const QString string = "9e-15";
    double value;
    double expected = 9e-15;

    QVERIFY(parseDouble(string, value));
    QVERIFY(std::abs(value-expected) <= epsilonWeak);
}

void DialogsToolsTests::parseDouble03()
{
    const QString string = "0.1.1 0";
    double value;

    QVERIFY(!parseDouble(string, value));
}

void DialogsToolsTests::parseInt01()
{
    const QString string = "123";
    int value;
    int expected = 123;

    QVERIFY(parseInt(string, value));
    QCOMPARE(value, expected);
}

void DialogsToolsTests::parseInt02()
{
    const QString string = "";
    int value;

    QVERIFY(!parseInt(string, value));
}

void DialogsToolsTests::parseInt03()
{
    const QString string = "0 1 1";
    int value;

    QVERIFY(!parseInt(string, value));
}

}
}
