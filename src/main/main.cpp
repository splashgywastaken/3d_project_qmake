#include "src/main/mainwindow/mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow/mainwindow.h"
#include "src/service/FileReaders/ObjFileReader/ObjFileReader.h"

#include "tests/MeshToolsTests/meshtoolstests.h"
#include "tests/ObjReaderTests/objreadertests.h"
#include "tests/KDTreeTests/kdtreetests.h"
#include <tests/OptimizationTests/optimizationtests.h>
#include "code playground/KDTreeCompareMethods/timetester.h"
#include "code playground/OptimizationPlayground/GradientDescentSolve/gradientdescentsolve.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    #ifndef QT_NO_DEBUG
    ObjReaderTests objReaderTests;
    QTest::qExec(&objReaderTests);
    MeshToolsTests meshToolsTests;
    QTest::qExec(&meshToolsTests);
    KDTree::KDTreeTests kdTreeTests;
    QTest::qExec(&kdTreeTests);
    Optimization::Tests::OptimizationTests optimizationTests;
    QTest::qExec(&optimizationTests);
    #else
    #endif

    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();

//    QString fileName = "E:/projects SSD/Qt/3d_project_qmake/res/obj/Car.obj";
//    QVector3D pointToSearch = {-0.5, 0.5, -0.5};

//    KDTree::TimeTester::testBuildTreeFromFile01(fileName, pointToSearch);

//    pointToSearch = {0.5, 0.5, 0.5};
//    KDTree::TimeTester::testBuildTreeFromFile01(fileName, pointToSearch);

//    fileName = "input file name";
//    pointToSearch = {0.5, 0.5, 0.5};
//    KDTree::TimeTester::testBuildTreeFromFile01(fileName, pointToSearch);

//    pointToSearch = {0.5, 0.5, -0.5};
//    KDTree::TimeTester::testBuildTreeFromFile01(fileName, pointToSearch);

//    // Function minimize
//    Optimization::GradientDescentSolve::solveMinimizeFunction();

    return 0;

}
