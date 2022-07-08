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

protected slots:
    void openObjFile();

    void addObject();
    void changeObjectColor();

    void changeVertexShader();
    void changeFragmentShader();

    void useNormalsCheckBoxClicked(bool checked);

protected:
    // Misc methods:
    void setLabelText(QLabel* label, QString text);
    void setLabelFontColor(QLabel* label, QString color);
    void changeShader();

    void createActions();
    void createMenus();

protected:
    // Menu bar actions
    QMenu* m_fileMenu;
    QMenu* m_objectMenu;

    // Files
    QAction* m_openAction;
    // Objects
    QAction* m_addObjectAction;
    QAction* m_changeObjectColorAction;

    // Custom widgets
    ObjectViewGLWidget * m_glWidget;

    // UI
    Ui::MainWindow *ui;

    // Other variables
    ObjReadingTools::ObjFileData *m_fileData;

    bool m_useNormals;
};
#endif // MAINWINDOW_H
