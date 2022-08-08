#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qdir.h"
#include <QMainWindow>
#include <findnearestpointdialog.h>
#include <qlabel.h>
#include <QWidgetAction>
#include <QComboBox>
#include <QColorDialog>
#include <qboxlayout.h>

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

public slots:
    void nearestPointFound(QVector3D nearestPoint);
    void setObjectColor(QColor objectColor);
    void setGridColor(QColor objectColor);
    void setBackgroundColor(QColor objectColor);

protected slots:
    // File menu:
    // Files
    void openObjFile();
    void addObjectSlot();

    // Scene
    void deleteLastObject();
    void clearObjects();

    // Shaders
    void changeShader(const QString &shader);

    // Objects
    void changeLastObjectColor();
    void findNearestPointInLastObject();
    void makeTargetObject();
    void performFittingforTarget();

    // View
    void changeBackgroundColor();
    void changeGridColor();
protected:
    // Misc methods:
    bool readObjectFromFile(
            ObjReadingTools::ObjFileData& destObj,
            const QString& caption,
            const QString& fileFilter,
            const QString& dir = QDir::homePath()
            );
    void addToSceneObject3DFromObjData(
            ObjReadingTools::ObjFileData& obj,
            SceneObject*& object,
            const QColor& color,
            ObjectViewGLWidget* glWidget
            );

    void setLabelText(QLabel* label, QString text);
    void setLabelFontColor(QLabel* label, QString color);

    void createActions();
    void createWidgetActions();
    void createMenus();

protected:
    // Dialogs
    QColorDialog* m_colorPickerDialog = nullptr;
    FindNearestPointDialog* m_findNearestPointDialog = nullptr;

    // Menu bar actions
    QMenu* m_fileMenu;
    QMenu* m_objectMenu;
    QMenu* m_shaderMenu;
    QMenu* m_sceneMenu;
    QMenu* m_instrumentsMenu;
    QMenu* m_viewMenu;

    // Files
    QAction* m_openAction;
    // Objects
    QAction* m_changeObjectColorAction;
    QAction* m_findNearestPointInLastObjectAction;
    // Shaders
    QMenu* m_switchShaderMenu;
    QWidgetAction* m_switchShaderWidgetAction;
    QComboBox* m_shaderComboBox;
    // Scene
    QAction* m_deleteLastObjectAction;
    QAction* m_clearObjectsAction;
    // Instruments
    QAction* m_makeTargetObjectAction;
    QAction* m_performFittingAction;
    // View menu
    QAction* m_changeBackgroundColorAction;
    QAction* m_changeGridColorAction;

    // Layouts
    QVBoxLayout* m_centralLayout;
    QHBoxLayout* m_openGLLayout;

    // OpenGL widget and corresponding data
    ObjectViewGLWidget * m_glWidget;

    // Event filters:
    CameraMovementEventFilter* m_cameraMovementEventFilter = nullptr;

    // UI
    Ui::MainWindow *m_ui;
    QProgressBar* m_taskProgressBar;

    // Other variables
    ObjReadingTools::ObjFileData *m_objDataBase = nullptr;
    ObjReadingTools::ObjFileData *m_objDataTarget = nullptr;
    ObjReadingTools::ObjFileData *m_objDataResult = nullptr;

    QColor* m_currentObjectColor = nullptr;
    SceneObject* m_current3DObject = nullptr;
    SceneObject* m_target3DObject = nullptr;
};
#endif // MAINWINDOW_H
