#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <src/models/dto/IObjFileData.h>
#include <src/service/handlers/OpenGL/openglhandler.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CustomGLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_searchButtonFilePath_clicked();

    void on_readButtonFilePath_clicked();

    void on_viewIbjectButton_clicked();

private:
    // Custom widgets
    CustomGLWidget *glWidget;

    // Other variables
    Ui::MainWindow *ui;
    IObjFileData *fileData;
    OpenGLHandler* opengGLHandler;
    QMap<QString, QVector<GLfloat>*>* arraysMap;
};
#endif // MAINWINDOW_H
