#include "mainwindow.h"

#include <QApplication>
#include <QTextEdit>

#include "src/main/mainwindow.h"
#include "src/models/dto/IObjFileData.h"
#include "src/service/file_readers/ObjFileReader/ObjFileReader.h"
#include "src/widgets/customglwidget.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto window = new MainWindow(nullptr);

    CustomGLWidget::setTransparent(false);
    if (CustomGLWidget::isTransparent()){
        window->setAttribute(Qt::WA_TranslucentBackground);
        window->setAttribute(Qt::WA_NoSystemBackground, false);
    }

    window->show();

    return app.exec();
}
