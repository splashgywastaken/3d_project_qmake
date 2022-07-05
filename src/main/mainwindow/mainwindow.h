#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>

#include <src/widgets/objectviewglwidget/objectviewglwidget.h>

#include <src/models/dto/ObjFileData/ObjFileData.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setObjectColor(QVector3D objectColor);

private slots:
    void openObjFile();

    void showObject();
    void changeObjectColor();

    void changeVertexShader();
    void changeFragmentShader();

    void reinitShaderProgram();
    void useNormalsCheckBoxClicked(bool checked);

private:
    // Misc methods:
    void setLabelText(QLabel* label, QString text);
    void setLabelFontColor(QLabel* label, QString color);

    void createActions();
    void createMenus();

    // Menu bar actions
    QMenu* fileMenu;
    QMenu* objectMenu;
    QMenu* shaderMenu;

    // Files
    QAction* openAction;
    // Objects
    QAction* showObjectAction;
    QAction* changeObjectColorAction;
    // Shaders
    QAction* changeVertexShaderAction;
    QAction* changeFragmentShaderAction;
    QAction* reinitShaderProgramAction;

    // Custom widgets
    ObjectViewGLWidget * glWidget;

    // UI
    Ui::MainWindow *ui;

    // Other variables
    ObjFileData *fileData;
};
#endif // MAINWINDOW_H
