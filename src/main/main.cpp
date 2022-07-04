#include "mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow.h"
#include "src/service/file_readers/ObjFileReader/ObjFileReader.h"

// 04.07.2022
// TODO: сделать работу прогресс баров через явную передачу прогресс бара (нотифаера) в методы
// TODO: настроить байнд для вершинного буффера
// 05.07.2022
// TODO: сделать код тестируемым и написать тесты под него

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();
}
