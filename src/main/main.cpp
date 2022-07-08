#include "src/main/mainwindow/mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow/mainwindow.h"
#include "src/service/file_readers/ObjFileReader/ObjFileReader.h"

#include "tests/MeshTools/meshtoolstests.h"
#include "tests/ObjReaderTests/objreadertests.h"

int main(int argc, char *argv[])
{
    ObjReaderTests objReaderTests;
    QTest::qExec(&objReaderTests);
    MeshToolsTests meshToolsTests;
    QTest::qExec(&meshToolsTests);

    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();
}
