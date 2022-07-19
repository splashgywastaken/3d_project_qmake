#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <findnearestpointdialog.h>
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

public slots:
    void nearestPointFound(QVector3D nearestPoint);

protected slots:
    // File menu:
    // Files
    void openObjFile();
    void addObject();

    // Scene
    void deleteLastObject();
    void clearObjects();

    // Objects
    void changeLastObjectColor();
    void findNearestPointInLastObject();
    void makeTargetObject();
    void performFittingforTarget();

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
    FindNearestPointDialog* m_findNearestPointDialog = nullptr;

    // Menu bar actions
    QMenu* m_fileMenu;
    QMenu* m_objectMenu;
    QMenu* m_sceneMenu;
    QMenu* m_instrumentsMenu;

    // Files
    QAction* m_openAction;
    // Objects
    QAction* m_changeObjectColorAction;
    QAction* m_findNearestPointInLastObjectAction;
    // Scene
    QAction* m_deleteLastObjectAction;
    QAction* m_clearObjectsAction;
    // Instruments
    QAction* m_makeTargetObjectAction;
    QAction* m_performFittingAction;

    // Custom widgets
    ObjectViewGLWidget * m_glWidget;

    // Event filters:
    CameraMovementEventFilter* m_cameraMovementEventFilter = nullptr;

    // UI
    Ui::MainWindow *m_ui;

    // Other variables
    ObjReadingTools::ObjFileData *m_objDataCurrent = nullptr;
    ObjReadingTools::ObjFileData *m_objDataTarget = nullptr;
    ObjReadingTools::ObjFileData *m_objDataResult = nullptr;

    Object3D* m_current3DObject = nullptr;
    Object3D* m_target3DObject = nullptr;

    bool m_useNormals;
    bool m_useNormalMap = false;
};
#endif // MAINWINDOW_H
