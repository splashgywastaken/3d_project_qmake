#include "src/main/mainwindow/mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow/mainwindow.h"
#include "src/service/FileReaders/ObjFileReader/ObjFileReader.h"

#include "tests/MeshToolsTests/meshtoolstests.h"
#include "tests/ObjReaderTests/objreadertests.h"
#include "tests/KDTreeTests/kdtreetests.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    ObjReaderTests objReaderTests;
    QTest::qExec(&objReaderTests);
    MeshToolsTests meshToolsTests;
    QTest::qExec(&meshToolsTests);
    KDTree::KDTreeTests kdTreeTests;
    QTest::qExec(&kdTreeTests);

    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();
}
