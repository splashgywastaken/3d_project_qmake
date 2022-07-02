#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>

#include <src/widgets/objectviewglwidget/objectviewglwidget.h>

#include <src/models/dto/ObjFileData/ObjFileData.h>

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
    void openObjFile();

    void showObject();

    void addObject();

private:
    // Misc methods:
    void setLabelText(QLabel* label, QString text);

    void createActions();
    void createMenus();

    // Menu bar actions
    QMenu* fileMenu;
    QMenu* objectMenu;

    QAction* openAction;
    QAction* showObjectAction;
    QAction* addObjectAction;

    // Custom widgets
    ObjectViewGLWidget * glWidget;
    //CustomGLWidget * glWidget;

    // Other variables
    Ui::MainWindow *ui;
    ObjFileData *fileData;
};
#endif // MAINWINDOW_H
