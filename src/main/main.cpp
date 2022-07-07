#include "src/main/mainwindow/mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow/mainwindow.h"
#include "src/service/file_readers/ObjFileReader/ObjFileReader.h"

#include "tests/MeshTools/meshtoolstests.h"
#include "tests/ObjReaderTests/objreadertests.h"

// 05.07.2022
// TODO: сделать код тестируемым и написать тесты под него

int main(int argc, char *argv[])
{

//    QVector3D vertex;
//    QVector2D vertex;
//    QString errorMsg;

//    if(ObjReadingTools::parseVector3D("1 2 3", vertex, errorMsg)){
//        qCritical() << QString("Failed. %1").arg(errorMsg);
//    }
//    QVector3D expectedVector(1, 2, 3);

//    if (ObjReadingTools::parseVector2D("1 2 3", vertex, errorMsg))
//    {
//        qCritical() << errorMsg;
//    }
//    QVector2D expectedVector(1, 2);

    ObjReaderTests objReaderTests;
    QTest::qExec(&objReaderTests);
    MeshToolsTests meshToolsTests;
    QTest::qExec(&meshToolsTests);

    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();
}
