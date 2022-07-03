#include "mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow.h"
#include "src/service/file_readers/ObjFileReader/ObjFileReader.h"
#include "src/widgets/customglwidget/customglwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    window->show();

    return app.exec();
}
