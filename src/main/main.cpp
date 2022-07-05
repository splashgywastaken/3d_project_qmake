#include "mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow.h"
#include "src/service/file_readers/ObjFileReader/ObjFileReader.h"

// 05.07.2022
// TODO: сделать код тестируемым и написать тесты под него

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();
}
