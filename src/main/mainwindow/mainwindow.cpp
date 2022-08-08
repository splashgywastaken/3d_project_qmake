#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QThread>
#include "qlayout.h"

#include <src/service/GlobalState.h>
#include <src/main/FindNearestPointDialog/findnearestpointdialog.h>
#include <src/service/Optimization/OptimizationUtils/optimizationutils.h>
#include <src/service/Optimization/LambdaStepCallback/lambdastepcallback.h>
#include <src/service/Optimization/GradientDescent/gradientdescent.h>
#include <src/service/RigidAlignment/RigidAlignmentProblem/rigidalignmentproblem.h>
#include <src/service/RigidAlignment/RigidAlignmentScalingProblem/rigidalignmentscalingproblem.h>
#include "src/service/KDTree/kdtree.h"
#include <src/widgets/objectviewglwidget/objectviewglwidget.h>

const QColor defaultSceneObjectColor(85, 170, 255);
const QColor defaultTargetObjectColor(255, 0, 0);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    // Variables setup

    // MenuBar setup:
    createActions();
    createMenus();
    createWidgetActions();

    // Layout inits
    m_centralLayout = new QVBoxLayout(m_ui->centralwidget);

    m_openGLLayout = new QHBoxLayout();
    m_centralLayout->addLayout(m_openGLLayout);

    // UI setup
    m_taskProgressBar = new QProgressBar();
    statusBar()->addPermanentWidget(m_taskProgressBar);
    m_taskProgressBar->disconnect();
    m_taskProgressBar->hide();

    // GlWidget inits
    m_glWidget = new ObjectViewGLWidget;
    m_openGLLayout->addWidget(m_glWidget);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::setObjectColor(QColor objectColor)
{
    m_currentObjectColor = new QColor(objectColor);
    m_glWidget->setObjectColor(objectColor);
    m_glWidget->update();
    m_colorPickerDialog->disconnect();
}

void MainWindow::setGridColor(QColor objectColor)
{
    m_glWidget->setGridColor(objectColor);
    m_glWidget->update();
    m_colorPickerDialog->disconnect();
}

void MainWindow::setBackgroundColor(QColor objectColor)
{
    m_glWidget->setBackgroundColor(objectColor);
    m_glWidget->update();
    m_colorPickerDialog->disconnect();
}

void MainWindow::openObjFile()
{
    m_objDataBase = new ObjReadingTools::ObjFileData();
    if (readObjectFromFile(*m_objDataBase, tr("Choose file"), tr("Object (*.obj)"), QDir::homePath()))
    {
        addObjectSlot();
    }
}

void MainWindow::addObjectSlot()
{
    addToSceneObject3DFromObjData(
                *m_objDataBase,
                m_current3DObject,
                defaultSceneObjectColor,
                m_glWidget
                );
    delete m_currentObjectColor;
    m_currentObjectColor = new QColor(defaultSceneObjectColor);
}

void MainWindow::deleteLastObject()
{
    m_glWidget->deleteLastObject();
    m_glWidget->update();
    m_glWidget->setObjectColor(m_glWidget->getObjectColor());
}

void MainWindow::clearObjects()
{
    delete m_target3DObject;
    m_target3DObject = nullptr;
    delete m_current3DObject;
    m_current3DObject = nullptr;
    delete m_objDataBase;
    m_objDataBase = nullptr;
    delete m_objDataTarget;
    m_objDataTarget = nullptr;
    delete m_objDataResult;
    m_objDataResult = nullptr;

    m_glWidget->clearObjects();
    m_glWidget->update();
    m_glWidget->setObjectColor(m_glWidget->getObjectColor());
}

void MainWindow::changeLastObjectColor()
{
    if (m_colorPickerDialog == nullptr)
    {
        m_colorPickerDialog = new QColorDialog(this);
    }
    m_colorPickerDialog->setCurrentColor(m_glWidget->getObjectColor());
    connect(m_colorPickerDialog, &QColorDialog::colorSelected, this, &MainWindow::setObjectColor);
    m_colorPickerDialog->show();
    m_colorPickerDialog->raise();
    m_colorPickerDialog->activateWindow();
}

void MainWindow::findNearestPointInLastObject()
{
    QVector<QVector3D> points = m_objDataBase->getVertices();

    KDTree::Node *kDTreeHead = KDTree::buildTree(points);
    m_findNearestPointDialog = new FindNearestPointDialog(points, kDTreeHead);

    connect(
            this->m_findNearestPointDialog, &FindNearestPointDialog::foundNearest,
            this, &MainWindow::nearestPointFound
            );
    m_findNearestPointDialog->show();
    m_findNearestPointDialog->raise();
    m_findNearestPointDialog->activateWindow();
}

void MainWindow::makeTargetObject()
{
    if (m_current3DObject == nullptr)
    {
        return;
    }

    QMatrix4x4 hardcodedTransformation;
    hardcodedTransformation.setToIdentity();

    hardcodedTransformation.scale(1.2);

    hardcodedTransformation.translate(QVector3D(0, 1, 0));

    hardcodedTransformation.rotate(90, QVector3D(1, 0, 0));
    hardcodedTransformation.rotate(90, QVector3D(0, 0, 1));

    if (m_objDataTarget == nullptr)
    {
        delete m_objDataTarget;
        m_objDataTarget = nullptr;
    }
    m_objDataTarget = new ObjReadingTools::ObjFileData();
    *m_objDataTarget = *m_objDataBase;

    int size = m_objDataTarget->getVertices().size();
    QVector<QVector3D> targetVertices = (m_objDataTarget->getVertices());
    for (int index = 0; index < size; index++)
    {
       targetVertices[index] = hardcodedTransformation.mapVector(targetVertices[index]);
    }
    m_objDataTarget->setVertices(targetVertices);

    if (m_target3DObject != nullptr)
    {
        m_glWidget->removeObject(m_target3DObject);
        delete m_target3DObject;
        m_target3DObject = nullptr;
    }
    addToSceneObject3DFromObjData(
                *m_objDataTarget,
                m_target3DObject,
                defaultTargetObjectColor,
                m_glWidget
                );
}

void MainWindow::performFittingforTarget()
{
    if (m_current3DObject == nullptr || m_target3DObject == nullptr)
        return;

    if (m_objDataResult != nullptr)
    {
        delete m_objDataResult;
        m_objDataResult = nullptr;
    }
    m_objDataResult = new ObjReadingTools::ObjFileData();

    const auto stepFunction = [&](const QVector<double>& variables)
    {
        *m_objDataResult = *m_objDataBase;
        const QMatrix4x4 transformation = Optimization::RigidAlignmentScalingProblem::transformationMatrixFromVars(variables);
        const int resultSize = m_objDataResult->getVertices().size();
        QVector<QVector3D> resultVertices = m_objDataResult->getVertices().toVector();
        for (int index = 0; index < resultSize; index++)
        {
            resultVertices[index] = transformation.mapVector(resultVertices[index]);
        }
        m_objDataResult->setVertices(resultVertices);

        m_glWidget->makeCurrent();
        if (m_current3DObject != nullptr)
        {
            m_glWidget->removeObject(m_current3DObject);
            delete m_current3DObject;
            m_current3DObject = nullptr;
        }
        addToSceneObject3DFromObjData(
                    *m_objDataResult,
                    m_current3DObject,
                    *m_currentObjectColor,
                    m_glWidget
                    );
        QApplication::processEvents();
        QThread::msleep(5);
    };

    Optimization::RigidAlignmentScalingProblem problem(
                m_objDataBase->getVertices(),
                m_objDataTarget->getVertices()
                );

    Optimization::LambdaStepCallback callback(stepFunction);

    const QVector<double> initialVariables = {0, 0, 0, 0, 0, 0, 1};
    const double stepLength = 0.4;
    const int nMaxIterations = 2500;
    const double gradientNormThreshold = 1e-10;

    QVector<double> resultTranslation = Optimization::gradientDescent(
                    problem, initialVariables,
                    stepLength, nMaxIterations, gradientNormThreshold,
                    true, &callback
                );

    qDebug() << "Estimated transformation" << Optimization::RigidAlignmentScalingProblem::transformationVectorFromVars(resultTranslation);
}

void MainWindow::changeBackgroundColor()
{
    if (m_colorPickerDialog == nullptr)
    {
        m_colorPickerDialog = new QColorDialog(this);
    }
    m_colorPickerDialog->setCurrentColor(m_glWidget->getBackgroundColor());
    connect(m_colorPickerDialog, &QColorDialog::colorSelected, this, &MainWindow::setBackgroundColor);
    m_colorPickerDialog->show();
    m_colorPickerDialog->raise();
    m_colorPickerDialog->activateWindow();
}

void MainWindow::changeGridColor()
{
    if (m_colorPickerDialog == nullptr)
    {
        m_colorPickerDialog = new QColorDialog(this);
    }
    m_colorPickerDialog->setCurrentColor(m_glWidget->getGridColor());
    connect(m_colorPickerDialog, &QColorDialog::colorSelected, this, &MainWindow::setGridColor);
    m_colorPickerDialog->show();
    m_colorPickerDialog->raise();
    m_colorPickerDialog->activateWindow();
}

bool MainWindow::readObjectFromFile(
        ObjReadingTools::ObjFileData &destObj,
        const QString &caption,
        const QString &fileFilter,
        const QString &dir
        )
{
    QFileDialog *fileDialog = new QFileDialog();
    QString resultFilePath(fileDialog->getOpenFileName(
                        this, caption,
                        dir,
                        fileFilter,
                        nullptr,
                        QFileDialog::DontUseNativeDialog
                    ));
    if (resultFilePath.isEmpty())
    {
        delete fileDialog;
        return false;
    }

    ProgressNotifierSingleton::initialize(m_taskProgressBar);
    AbstractProgressNotifier* progressNotifier = ProgressNotifierSingleton::getInstance();
    QString errorMessage;

    if (!ObjReadingTools::readFile(resultFilePath, destObj, errorMessage, progressNotifier)){
        delete fileDialog;
        qDebug() << errorMessage;
        return false;
    }

    delete fileDialog;
    return true;
}

void MainWindow::addToSceneObject3DFromObjData(
        ObjReadingTools::ObjFileData& obj,
        SceneObject*& object,
        const QColor& color,
        ObjectViewGLWidget* glWidget
        )
{
    QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(obj.getPolygonVertexIndices());
    QVector<int> polygonNormalIndices = MeshTools::buildPolygonVertexIndicesVector(obj.getPolygonNormalIndices());
    QVector<int> polygonStart = MeshTools::buildPolygonStartVector(obj.getPolygonVertexIndices());

    object = new Object3D(
                obj.getVertices(),
                polygonVertexIndices,
                polygonStart,
                obj.getNormals(),
                polygonNormalIndices
                );

    glWidget->makeCurrent();
    glWidget->addObject(object);
    glWidget->setObjectColor(color);
    glWidget->update();
}

void MainWindow::nearestPointFound(QVector3D nearestPoint)
{
    m_glWidget->addPoint(nearestPoint);
}

void MainWindow::setLabelText(QLabel *label, QString text)
{
    label->setText(text);
}

void MainWindow::setLabelFontColor(QLabel *label, QString color)
{
    label->setStyleSheet("color: " + color);
}

void MainWindow::changeShader(const QString &shaderName)
{
    DrawableObjectTools::ShaderProgrammType shaderType = DrawableObjectTools::ShaderProgrammType::Standard;

    #ifndef QT_NO_DEBUG
        qDebug() << "Change shader triggered";
    #endif

    if (shaderName == "Basic shader")
    {
        shaderType = DrawableObjectTools::ShaderProgrammType::Standard;
    }
    else if (shaderName == "Lightning shader")
    {
        shaderType = DrawableObjectTools::ShaderProgrammType::Lightning;
    }
    else if (shaderName == "Normal map shader")
    {
        shaderType = DrawableObjectTools::ShaderProgrammType::NormalMap;
    }
    else if (shaderName == "Lightning shader with textures")
    {
        shaderType = DrawableObjectTools::ShaderProgrammType::LightningWithTextures;
    }

    m_glWidget->switchShaders(shaderType);
    m_glWidget->update();
}

void MainWindow::createActions()
{
    // File's menu actions
    // Open file Action
    m_openAction = new QAction(tr("Open file"), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open a new file"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openObjFile);

    // Object's menu actions
    // Change object color
    m_changeObjectColorAction = new QAction(tr("Change object color"), this);
    m_changeObjectColorAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C));
    m_changeObjectColorAction->setToolTip(tr("Use color pallete to change object color"));
    connect(m_changeObjectColorAction, &QAction::triggered, this, &MainWindow::changeLastObjectColor);

    // Find nearest point index in last object
    m_findNearestPointInLastObjectAction = new QAction(tr("Find nearest point"), this);
    m_findNearestPointInLastObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F));
    m_findNearestPointInLastObjectAction->setToolTip(tr("Use dialog window to input data and find corresponding point in object"));
    connect(m_findNearestPointInLastObjectAction, &QAction::triggered, this, &MainWindow::findNearestPointInLastObject);

    // Scene's menu actions
    // Delete last object
    m_deleteLastObjectAction = new QAction(tr("Delete last object"), this);
    m_deleteLastObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_X));
    m_deleteLastObjectAction->setToolTip(tr("Delete last object that you added on scene"));
    connect(m_deleteLastObjectAction, &QAction::triggered, this, &MainWindow::deleteLastObject);

    // Clear objects from scene
    m_clearObjectsAction = new QAction(tr("Clear scene"), this);
    m_clearObjectsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_X));
    m_clearObjectsAction->setToolTip(tr("Clear all objects except "));
    connect(m_clearObjectsAction, &QAction::triggered, this, &MainWindow::clearObjects);

    // Instrument's menu actions
    // Make target to fit
    m_makeTargetObjectAction = new QAction(tr("Make target object"), this);
    m_makeTargetObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_T));
    m_makeTargetObjectAction->setToolTip(tr("Make target for object to fit"));
    connect(m_makeTargetObjectAction, &QAction::triggered, this, &MainWindow::makeTargetObject);

    // Perform fitting for current target
    m_performFittingAction = new QAction(tr("Perform fitting for object"), this);
    m_performFittingAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_F));
    m_performFittingAction->setToolTip(tr("Perform fitting for current target object"));
    connect(m_performFittingAction, &QAction::triggered, this, &MainWindow::performFittingforTarget);

    // View's menu actions
    // Change background's color
    m_changeBackgroundColorAction = new QAction(tr("Change background color"), this);
    connect(m_changeBackgroundColorAction, &QAction::triggered, this, &MainWindow::changeBackgroundColor);

    m_changeGridColorAction = new QAction(tr("Change grid color"), this);
    connect(m_changeGridColorAction, &QAction::triggered, this, &MainWindow::changeGridColor);
}

void MainWindow::createWidgetActions()
{
    // Shader combo box
    m_shaderComboBox = new QComboBox(m_shaderMenu);
    m_switchShaderWidgetAction = new QWidgetAction(m_shaderMenu);
    using namespace DrawableObjectTools;
    m_shaderComboBox->addItems(QStringList({
                                          "Basic shader",
                                          "Lightning shader",
                                          "Normal map shader",
                                          "Lightning shader with textures"
                                      }));
    m_shaderComboBox->setCurrentText("Lightning shader");

    m_switchShaderWidgetAction->setDefaultWidget(m_shaderComboBox);
    QObject::connect(m_shaderComboBox, &QComboBox::currentTextChanged, this, &MainWindow::changeShader);

    m_switchShaderMenu->addAction(m_switchShaderWidgetAction);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);

    m_objectMenu = menuBar()->addMenu(tr("&Object"));
    m_objectMenu->addAction(m_changeObjectColorAction);
    m_objectMenu->addAction(m_findNearestPointInLastObjectAction);

    // Shader's menu actions
    m_switchShaderMenu = new QMenu(tr("Change shader"), this);
    m_switchShaderMenu->setToolTip(tr("Use list of shaders to switch your current one"));

    m_shaderMenu = menuBar()->addMenu(tr("&Shaders"));
    m_shaderMenu->addMenu(m_switchShaderMenu);

    m_sceneMenu = menuBar()->addMenu(tr("&Scene"));
    m_sceneMenu->addAction(m_deleteLastObjectAction);
    m_sceneMenu->addAction(m_clearObjectsAction);

    m_instrumentsMenu = menuBar()->addMenu(tr("&Instruments"));
    m_instrumentsMenu->addAction(m_makeTargetObjectAction);
    m_instrumentsMenu->addAction(m_performFittingAction);

    m_viewMenu = menuBar()->addMenu(tr("View"));
    m_viewMenu->addAction(m_changeBackgroundColorAction);
    m_viewMenu->addAction(m_changeGridColorAction);
}
