#ifndef OPTIMIZATION_TESTS_OPTIMIZATIONTESTS_H
#define OPTIMIZATION_TESTS_OPTIMIZATIONTESTS_H

#include <QTest>

namespace Optimization {
namespace Tests {

class OptimizationTests : public QObject
{
    Q_OBJECT
private slots:
    void testUtilsAddVectors01();

    void testUtilsSubVectors01();

    void testUtilsMulVectorByConstant01();

    void testUtilsVectorNorm01();

    void testUtilsNumericalDerivative01();
    void testUtilsNumericalDerivative02();
    void testUtilsNumericalDerivative03();
    void testUtilsNumericalDerivative04();
    void testUtilsNumericalDerivative05();
    void testUtilsNumericalDerivative06();

    void testGradientDescent01();
    void testGradientDescent02();
    void testGradientDescent03();
    void testGradientDescent04();
    void testGradientDescent05();
    void testGradientDescent06();
    void testGradientDescent07();
};

}
}

#endif
