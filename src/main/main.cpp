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

//    CustomGLWidget::setTransparent(false);
//    if (CustomGLWidget::isTransparent()){
//        window->setAttribute(Qt::WA_TranslucentBackground);
//        window->setAttribute(Qt::WA_NoSystemBackground, false);
//    }

    // TODO: переделать внешний вид окна, сделать все основные кнопки через меню в вершине окна, а потом по гайду из видоса сделать работу с OpenGL

    window->show();

    return app.exec();
}
