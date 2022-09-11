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
#include <QMessageBox>

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
    void loadObjectFromObjFile(bool value);

    // Scene
    void deleteLastObject(bool value);
    void clearObjects(bool value);

    // Shaders
    void changeShader(const QString &shader);

    // Objects
    void changeLastObjectColor(bool value);
    void findNearestPointInLastObject(bool value);

    // Fitting
    void loadFittingBaseObject(bool value);
    void makeFittingTargetObject(bool value);
    void performFittingforTarget(bool value);
    void performClosestPointsBasedFitting(bool value);

    // Registration
    void loadRegistrationBaseMeshObject(bool value);
    void loadRegistrationTargetMeshObject(bool value);
    void performRigidRegistration(bool value);

    // View
    void changeBackgroundColor(bool value);
    void changeGridColor(bool value);
protected:
    // Misc methods:
    bool readObjectFromFile(
            ObjReadingTools::ObjFileData& destObj,
            const QString& caption,
            const QString& fileFilter,
            QString& errorMessage,
            const QString& dir = QDir::homePath()
            );
    void addSceneObjectFromObjData(
            ObjReadingTools::ObjFileData& obj,
            SceneObject*& object,
            const QColor& color,
            ObjectViewGLWidget* glWidget
            );
    void addNormalsToObjData(ObjReadingTools::ObjFileData& objData);

    void setLabelText(QLabel* label, QString text);
    void setLabelFontColor(QLabel* label, QString color);

    void createActions();
    void setupAction(
            QAction*& action,
            QObject* actionsParent,
            const QString& caption,
            const QKeySequence& keySequence,
            const QString& toolTip,
            QObject* connectionReciever,
            const char* connectionSlot
            );

    void createWidgetActions();

    void createMenus();
    void setupMenu(
            QMenu*& menu,
            const QString& caption,
            const QString& toolTip,
            QList<QAction*> actionsList
            );

    // Fills message box with data to display. Writes values from result vector to display transformation info
    // Main text is the text that goes after info about transformation
    void setupYesNoTransformMessageBox(
            QMessageBox& yesNoMessageBox,
            QString windowTitle,
            QString mainText,
            QVector<double> resultVector = {}
            );

protected:
    // Dialogs
    QColorDialog* m_colorPickerDialog = nullptr;
    FindNearestPointDialog* m_findNearestPointDialog = nullptr;

    // Menu bar actions
    QMenu* m_fileMenu;
    QMenu* m_objectMenu;
    QMenu* m_shaderMenu;
    QMenu* m_sceneMenu;
    QMenu* m_fittingMenu;
    QMenu* m_registrationMenu;
    QMenu* m_viewMenu;

    // Files
    QAction* m_loadObjectFromObjFileAction;
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
    // Fitting
    QAction* m_loadFittingBaseObjectActon;
    QAction* m_makeFittingTargetObjectAction;
    QAction* m_performFittingAction;
    QAction* m_performClosestPointsBasedFitting;
    // Registration
    QAction* m_loadRegistrationBaseMeshObjectAction;
    QAction* m_loadRegistrationTargetObjectAction;
    QAction* m_performRigidRegistrationAction;
    // View menu
    QAction* m_changeBackgroundColorAction;
    QAction* m_changeGridColorAction;

    // Layouts
    QVBoxLayout* m_centralLayout;
    QHBoxLayout* m_openGLLayout;

    // OpenGL widget and corresponding data
    ObjectViewGLWidget * m_glWidget;

    // UI
    Ui::MainWindow *m_ui;
    QProgressBar* m_taskProgressBar;

    // Event filters:
    CameraMovementEventFilter* m_cameraMovementEventFilter = nullptr;

    // Other variables
    // Base scene object data
    ObjReadingTools::ObjFileData *m_objDataBase = nullptr;

    // Fitting object data
    ObjReadingTools::ObjFileData *m_fittingObjDataBase = nullptr;
    ObjReadingTools::ObjFileData *m_fittingObjDataTarget = nullptr;
    ObjReadingTools::ObjFileData *m_fittingObjDataResult = nullptr;
    // Registration object data
    ObjReadingTools::ObjFileData *m_registrationObjDataBase = nullptr;
    ObjReadingTools::ObjFileData *m_registrationObjDataTarget = nullptr;
    ObjReadingTools::ObjFileData *m_registrationObjDataResult = nullptr;

    // Base scene object
    SceneObject* m_baseSceneObject = nullptr;

    // Fitting objects
    SceneObject* m_fittingBaseSceneObject = nullptr;
    SceneObject* m_fittingTargetSceneObject = nullptr;

    // Registration objects
    SceneObject* m_registrationBaseMeshSceneObject = nullptr;
    SceneObject* m_registrationTargetMeshSceneObject = nullptr;

    QColor* m_currentObjectColor = nullptr;
};
#endif // MAINWINDOW_H
