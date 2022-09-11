#include "optimizationtests.h"

#include <cmath>
#include <rigidalignmentscalingproblem.h>
#include "qmatrix4x4.h"
#include "src/service/Optimization/Problem/problem.h"
#include "src/service/Optimization/OptimizationUtils/optimizationutils.h"
#include "src/service/Optimization/GradientDescent/gradientdescent.h"
#include <src/service/Optimization/LambdaProblem/lambdaproblem.h>

const double epsilonHard = 1e-7;
const double epsilonWeak = 1e-4;

void Optimization::Tests::OptimizationTests::testUtilsAddVectors01()
{
    const QVector<double> a = {1, 2, 3};
    const QVector<double> b = {10, 20, 30};
    const QVector<double> res = Optimization::addVectors(a, b);
    QCOMPARE(3, res.size());
    QVERIFY(std::abs(res[0] - 11) < epsilonHard);
    QVERIFY(std::abs(res[1] - 22) < epsilonHard);
    QVERIFY(std::abs(res[2] - 33) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsSubVectors01()
{
    const QVector<double> a = {1, 2, 3};
    const QVector<double> b = {10, 20, 30};
    const QVector<double> res = Optimization::subVectors(a, b);
    QCOMPARE(3, res.size());
    QVERIFY(std::abs(res[0] + 9) < epsilonHard);
    QVERIFY(std::abs(res[1] + 18) < epsilonHard);
    QVERIFY(std::abs(res[2] + 27) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsMulVectorByConstant01()
{
    const QVector<double> vec = {-1, 20};
    const double scalar = 0.1;

    const QVector<double> res = Optimization::mulVectorByScalar(scalar, vec);
    QCOMPARE(2, res.size());
    QVERIFY(std::abs(res[0] + 0.1) < epsilonHard);
    QVERIFY(std::abs(res[1] - 2) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsVectorNorm01()
{
    const QVector<double> vec = {4, 3};
    const double res = Optimization::vectorNorm(vec);
    QVERIFY(std::abs(res - 5) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsNumericalDerivative01()
{
    const auto function = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return vars[0] * 2;
    };

    const QVector<double> vars = {10};
    const QVector<double> res = Optimization::numericalGradient(function, vars, 1);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0] - 2) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsNumericalDerivative02()
{
    const auto function = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 2);
        return -2 * vars[0] + 6 * vars[1];
    };

    const QVector<double> vars = {10, 100};
    const QVector<double> res = Optimization::numericalGradient(function, vars, 1);

    QCOMPARE(2, res.size());
    QVERIFY(std::abs(res[0] + 2) < epsilonHard);
    QVERIFY(std::abs(res[1] - 6) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsNumericalDerivative03()
{
    const auto function = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return vars[0] * vars[0] + 10;
    };

    const QVector<double> vars = {0};
    const QVector<double> res = Optimization::numericalGradient(function, vars, 1);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0]) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsNumericalDerivative04()
{
    const auto function = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return vars[0] * vars[0] + 10;
    };

    const QVector<double> vars = {0.5};
    const QVector<double> res = Optimization::numericalGradient(function, vars, 1e-8);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0] - 1.0) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsNumericalDerivative05()
{
    const auto function = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return 2 * vars[0] * vars[0] + 10;
    };

    const QVector<double> vars = {-0.5};
    const QVector<double> res = Optimization::numericalGradient(function, vars, 1e-8);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0] + 2.0) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testUtilsNumericalDerivative06()
{
    const auto function = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return 2 * vars[0] * vars[0] + 4 * vars[0] + 10;
    };

    const QVector<double> vars = {-0.5};
    const QVector<double> res = Optimization::numericalGradient(function, vars, 1e-8);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0] - 2.0) < epsilonHard);
}

void Optimization::Tests::OptimizationTests::testGradientDescent01()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return vars[0] * vars[0];
    };

    const auto gradientFunc = [](const QVector<double> &vars) -> QVector<double> {
        Q_ASSERT(vars.size() == 1);
        return {2 * vars[0]};
    };
    Optimization::Problem *problem = new Optimization::LambdaProblem(errorFunc, gradientFunc);

    const QVector<double> initialVars = {6};
    const QVector<double> res = Optimization::gradientDescent(problem, initialVars, 1e-1, 100, 1e-7, false);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0]) < epsilonWeak);
}

void Optimization::Tests::OptimizationTests::testGradientDescent02()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return vars[0] * vars[0];
    };

    const auto gradientFunc = [=](const QVector<double> &vars) -> QVector<double> {
        Q_ASSERT(vars.size() == 1);
        return numericalGradient(errorFunc, vars, 1e-8);
    };
    Optimization::Problem *problem = new Optimization::LambdaProblem(errorFunc, gradientFunc);

    const QVector<double> initialVars = {6};
    const QVector<double> res = Optimization::gradientDescent(problem, initialVars, 1e-1, 100, 1e-7, false);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0]) < epsilonWeak);
}

void Optimization::Tests::OptimizationTests::testGradientDescent03()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return vars[0] * vars[0] - vars[0] * 3 + 10;
    };

    const auto gradientFunc = [=](const QVector<double> &vars) -> QVector<double> {
        Q_ASSERT(vars.size() == 1);
        return numericalGradient(errorFunc, vars, 1e-8);
    };
    Optimization::Problem *problem = new Optimization::LambdaProblem(errorFunc, gradientFunc);

    const QVector<double> initialVars = {-6};
    const QVector<double> res = Optimization::gradientDescent(problem, initialVars, 1e-1, 100, 1e-7, false);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0] - 1.5) < epsilonWeak);
}

void Optimization::Tests::OptimizationTests::testGradientDescent04()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 1);
        return std::sin(vars[0]);
    };

    const auto gradientFunc = [=](const QVector<double> &vars) -> QVector<double> {
        Q_ASSERT(vars.size() == 1);
        return numericalGradient(errorFunc, vars, 1e-8);
    };
    Optimization::Problem *problem = new Optimization::LambdaProblem(errorFunc, gradientFunc);

    const QVector<double> initialVars = {0.1};
    const QVector<double> res = Optimization::gradientDescent(problem, initialVars, 1e-1, 100, 1e-7, false);

    QCOMPARE(1, res.size());
    QVERIFY(std::abs(res[0] + M_PI / 2.0) < epsilonWeak);
}

void Optimization::Tests::OptimizationTests::testGradientDescent05()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 2);
        return vars[0] * vars[0] + vars[1] * vars[1];
    };

    const auto gradientFunc = [=](const QVector<double> &vars) -> QVector<double> {
        return numericalGradient(errorFunc, vars, 1e-8);
    };
    Optimization::Problem *problem = new Optimization::LambdaProblem(errorFunc, gradientFunc);

    const QVector<double> initialVars = {-6, -3};
    const QVector<double> res = Optimization::gradientDescent(problem, initialVars, 1e-1, 100, 1e-7, false);

    QCOMPARE(2, res.size());
    QVERIFY(std::abs(res[0] - 0.0) < epsilonWeak);
    QVERIFY(std::abs(res[1] - 0.0) < epsilonWeak);
}

void Optimization::Tests::OptimizationTests::testGradientDescent06()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 2);
        return vars[0] * vars[0] + vars[1] * vars[1] - 6 * vars[1];
    };

    const auto gradientFunc = [=](const QVector<double> &vars) -> QVector<double> {
        return numericalGradient(errorFunc, vars, 1e-8);
    };
    Optimization::Problem *problem = new Optimization::LambdaProblem(errorFunc, gradientFunc);

    const QVector<double> initialVars = {-6, -3};
    const QVector<double> res = Optimization::gradientDescent(problem, initialVars, 1e-1, 100, 1e-7, false);

    QCOMPARE(2, res.size());
    QVERIFY(std::abs(res[0] - 0.0) < epsilonWeak);
    QVERIFY(std::abs(res[1] - 3.0) < epsilonWeak);
}

void Optimization::Tests::OptimizationTests::testGradientDescent07()
{
    const auto errorFunc = [](const QVector<double> &vars) -> double {
        Q_ASSERT(vars.size() == 2);
        return vars[0] * vars[0] + vars[1] * vars[1] - 0.5 * vars[0] * vars[1] + 3 * vars[0];
    };

    const auto gradientFunc = [=](const QVector<double> &vars) -> QVector<double> {
        return numericalGradient(errorFunc, vars, 1e-8);
    };
    Optimization::Problem *problem = new Optimization::LambdaProblem(errorFunc, gradientFunc);

    const QVector<double> initialVars = {-6, -3};
    const QVector<double> res = Optimization::gradientDescent(problem, initialVars, 1e-1, 100, 1e-7, false);

    QCOMPARE(2, res.size());
    QVERIFY(std::abs(res[0] + 1.6) < epsilonWeak);
    QVERIFY(std::abs(res[1] + 0.4) < epsilonWeak);
}
