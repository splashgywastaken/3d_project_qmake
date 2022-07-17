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
    void deleteLastObject();
    void clearObjects();
    void changeLastObjectColor();

    void useNormalsCheckBoxClicked(bool checked);
    void useNormalMapCheckBoxClicked(bool checked);
protected:
    // Misc methods:
    void setLabelText(QLabel* label, QString text);
    void setLabelFontColor(QLabel* label, QString color);
    void changeShader();

    void createActions();
    void createMenus();

protected:
    // Dialogs
    QDialog* m_colorPickerDialog = nullptr;

    // Menu bar actions
    QMenu* m_fileMenu;
    QMenu* m_objectMenu;
    QMenu* m_sceneMenu;

    // Files
    QAction* m_openAction;
    // Objects
    QAction* m_changeObjectColorAction;
    // Scene
    QAction* m_deleteLastObjectAction;
    QAction* m_clearObjectsAction;

    // Custom widgets
    ObjectViewGLWidget * m_glWidget;

    // Event filters:
    CameraMovementEventFilter* m_cameraMovementEventFilter = nullptr;

    // UI
    Ui::MainWindow *m_ui;

    // Other variables
    ObjReadingTools::ObjFileData *m_fileData;

    bool m_useNormals;
    bool m_useNormalMap = false;
};
#endif // MAINWINDOW_H
