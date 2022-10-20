#include "src/main/mainwindow/mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow/mainwindow.h"
#include "src/service/FileReaders/ObjFileReader/ObjFileReader.h"

#include "tests/MeshToolsTests/meshtoolstests.h"
#include "tests/ObjReaderTests/objreadertests.h"
#include "tests/KDTreeTests/kdtreetests.h"
#include "tests/OptimizationTests/optimizationtests.h"
#include "tests/GeomToolsTests/geomtoolstests.h"
#include "tests/QVectorToolsTests/qvectortoolstests.h"
#include "tests/DialogsToolsTests/dialogstoolstests.h"
#include "code playground/KDTreeCompareMethods/timetester.h"
#include "code playground/OptimizationPlayground/GradientDescentSolve/gradientdescentsolve.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    #ifdef QT_DEBUG
    ObjReaderTests objReaderTests;
    QTest::qExec(&objReaderTests);
    MeshToolsTests meshToolsTests;
    QTest::qExec(&meshToolsTests);
    KDTree::KDTreeTests kdTreeTests;
    QTest::qExec(&kdTreeTests);
    Optimization::Tests::OptimizationTests optimizationTests;
    QTest::qExec(&optimizationTests);
    QVectorTools::Tests::QVectorToolsTests qVectorToolsTests;
    QTest::qExec(&qVectorToolsTests);
    GeomTools::Tests::GeomToolsTests geomToolsTests;
    QTest::qExec(&geomToolsTests);
    DialogsTools::Tests::DialogsToolsTests dialogsToolsTests;
    QTest::qExec(&dialogsToolsTests);
    #else
    #endif

    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();

    return 0;
}
