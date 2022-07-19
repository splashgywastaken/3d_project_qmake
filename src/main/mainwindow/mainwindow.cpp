#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

#include <src/service/GlobalState.h>
#include <src/main/colorpicker/colorpicker.h>
#include <src/main/FindNearestPointDialog/findnearestpointdialog.h>
#include <src/service/Optimization/OptimizationUtils/optimizationutils.h>
#include <src/service/Optimization/LambdaStepCallback/lambdastepcallback.h>
#include <src/service/Optimization/GradientDescent/gradientdescent.h>
#include <src/service/RigidAlignment/RigidAlignmentProblem/rigidalignmentproblem.h>
#include "src/service/KDTree/kdtree.h"
#include <src/widgets/objectviewglwidget/objectviewglwidget.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    // Variables setup
    m_objDataCurrent = nullptr;
    m_useNormals = m_ui->useNormalsCheckBox->isChecked();

    // MenuBar setup:
    createActions();
    createMenus();

    // UI setup
    m_ui->taskProgressBar->setVisible(false);
    m_ui->taskLabel->setVisible(false);

    // GlWidget inits
    m_glWidget = new ObjectViewGLWidget;

    m_ui->openGLLayout->addWidget(m_glWidget);

    // Slots connection
    connect(m_ui->useNormalsCheckBox, &QCheckBox::toggled, this, &MainWindow::useNormalsCheckBoxClicked);
    connect(m_ui->useNormalMapCheckBox, &QCheckBox::toggled, this, &MainWindow::useNormalMapCheckBoxClicked);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::setObjectColor(QVector3D objectColor)
{
    m_glWidget->setObjectColor(objectColor);
    m_glWidget->update();
}

void MainWindow::openObjFile()
{
    QString filePath(
                QFileDialog::getOpenFileName(this, tr("Choose file"),
                "E:/projects SSD/Qt/3d_project_qmake/res/obj/cube.obj",
                tr("Object (*.obj)")
            ));

    if (filePath.isEmpty())
    {
        return;
    }

    m_ui->taskProgressBar->setVisible(true);
    m_ui->taskLabel->setVisible(true);
    setLabelFontColor(m_ui->taskLabel, "yellow");
    setLabelText(m_ui->taskLabel, "Reading file: " + filePath);

    ProgressNotifierSingleton::initialize(m_ui->taskProgressBar);
    AbstractProgressNotifier* progressNotifier = ProgressNotifierSingleton::getInstance();
    m_objDataCurrent = new ObjReadingTools::ObjFileData();
    QString errorMessage;

    if (!ObjReadingTools::readFile(filePath, *m_objDataCurrent, errorMessage, progressNotifier)){
        setLabelFontColor(m_ui->taskLabel, "red");
        setLabelText(m_ui->taskLabel, errorMessage);
        qDebug() << errorMessage;
        return;
    } else {
        // If reading was successfull
        setLabelFontColor(m_ui->taskLabel, "green");
        setLabelText(m_ui->taskLabel, "File successfully read");
        setLabelText(m_ui->objectNameLabel, m_objDataCurrent->getObjectName());
    }

    addObject();
}

void MainWindow::addObject()
{
    if (m_objDataCurrent == nullptr)
    {
        QMessageBox::warning(
                    this,
                    "Объект не прочитан",
                    "Данные об объекте пусты"
                    );
        return;
    }

    QMessageBox taskMessageBox;
    taskMessageBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setLabelFontColor(m_ui->taskLabel, "yellow");
    setLabelText(m_ui->taskLabel, "Processing Object");

    QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(m_objDataCurrent->getPolygonVertexIndices());
    QVector<int> polygonNormalIndices = MeshTools::buildPolygonVertexIndicesVector(m_objDataCurrent->getPolygonNormalIndices());
    QVector<int> polygonStart = MeshTools::buildPolygonStartVector(m_objDataCurrent->getPolygonVertexIndices());

    m_current3DObject = new Object3D(
                m_objDataCurrent->getVertices(),
                polygonVertexIndices,
                polygonStart,
                m_objDataCurrent->getNormals(),
                polygonNormalIndices
                );

    m_glWidget->makeCurrent();
    m_glWidget->addObject(m_current3DObject);
    m_glWidget->setObjectColor(QVector3D(1, 0, 0));

    setLabelFontColor(m_ui->taskLabel, "green");
    setLabelText(m_ui->taskLabel, "Object added");

    m_glWidget->update();
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
    delete m_objDataCurrent;
    m_objDataCurrent = nullptr;
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
    QVector3D objectColor = m_glWidget->getObjectColor();
    if (m_colorPickerDialog != nullptr)
    {
        delete m_colorPickerDialog;
    }
    m_colorPickerDialog = new ColorPicker(this, this, objectColor);
    m_colorPickerDialog->show();
    m_colorPickerDialog->raise();
    m_colorPickerDialog->activateWindow();
}

void MainWindow::findNearestPointInLastObject()
{
    QVector<QVector3D> points = m_objDataCurrent->getVertices();

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
    hardcodedTransformation.translate(0, 1, 0);
    hardcodedTransformation.rotate(90, QVector3D(1, 0, 0));
    hardcodedTransformation.rotate(90, QVector3D(0, 0, 1));

    if (m_objDataTarget == nullptr)
    {
        delete m_objDataTarget;
        m_objDataTarget = nullptr;
    }
    m_objDataTarget = new ObjReadingTools::ObjFileData();
    *m_objDataTarget = *m_objDataCurrent;

    int size = m_objDataTarget->getVertices().size();
    QVector<QVector3D> targetVertices = m_objDataTarget->getVertices();
    for (int index = 0; index < size; index++)
    {
       targetVertices[index] = hardcodedTransformation.mapVector(targetVertices[index]);
    }
    m_objDataTarget->setVertices(targetVertices);

    QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(m_objDataTarget->getPolygonVertexIndices());
    QVector<int> polygonNormalIndices = MeshTools::buildPolygonVertexIndicesVector(m_objDataTarget->getPolygonNormalIndices());
    QVector<int> polygonStart = MeshTools::buildPolygonStartVector(m_objDataTarget->getPolygonVertexIndices());
    m_glWidget->makeCurrent();
    if (m_target3DObject != nullptr)
    {
        m_glWidget->removeObject(m_target3DObject);
        delete m_target3DObject;
        m_target3DObject = nullptr;
    }
    m_target3DObject = new Object3D(
                    m_objDataTarget->getVertices(), polygonVertexIndices,
                    polygonStart,
                    m_objDataTarget->getNormals(), polygonNormalIndices
                );
    m_glWidget->addObject(m_target3DObject);
    m_glWidget->update();
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
    *m_objDataResult = *m_objDataCurrent;

    const auto stepFunction = [&](const QVector<double>& variables)
    {
        const QMatrix4x4 transformation = Optimization::RigidAlignmentProblem::transformationMatrixFromVars(variables);
        const int resultSize = m_objDataResult->getVertices().size();
        QVector<QVector3D> resultVertices = m_objDataResult->getVertices();
        for (int index = 0; index < resultSize; index++)
        {
            resultVertices[index] = transformation.mapVector(resultVertices[index]);
        }
        m_objDataResult->setVertices(resultVertices);

        QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(m_objDataResult->getPolygonVertexIndices());
        QVector<int> polygonNormalIndices = MeshTools::buildPolygonVertexIndicesVector(m_objDataResult->getPolygonNormalIndices());
        QVector<int> polygonStart = MeshTools::buildPolygonStartVector(m_objDataResult->getPolygonVertexIndices());

        m_glWidget->makeCurrent();
        if (m_current3DObject != nullptr)
        {
            m_glWidget->removeObject(m_current3DObject);
            delete m_current3DObject;
            m_current3DObject = nullptr;
        }
        m_current3DObject = new Object3D(
                    m_objDataResult->getVertices(), polygonVertexIndices,
                    polygonStart,
                    m_objDataResult->getNormals(), polygonNormalIndices
                );

        m_glWidget->addObject(m_current3DObject);
        m_glWidget->update();

        QApplication::processEvents();
        QThread::msleep(5);
    };

    Optimization::RigidAlignmentProblem problem(
                m_objDataCurrent->getVertices(),
                m_objDataCurrent->getVertices()
                );

    Optimization::LambdaStepCallback callback(stepFunction);

    const QVector<double> initialVariables = {0, 0, 0, 0, 0, 0};
    const double stepLength = 0.8;
    const int nMaxIterations = 15000;
    const double gradientNormThreshold = 1e-10;

    const QVector<double> resultTranslation = Optimization::gradientDescent(
                    problem, initialVariables,
                    stepLength, nMaxIterations, gradientNormThreshold,
                    true, &callback
                );
    qDebug() << "Estimated transformation" << resultTranslation;
}

void MainWindow::useNormalsCheckBoxClicked(bool checked)
{
    m_useNormals = checked;
    changeShader();
}

void MainWindow::useNormalMapCheckBoxClicked(bool checked)
{
    m_useNormalMap = checked;

    m_ui->useNormalsCheckBox->setCheckable(!m_useNormalMap);
    m_ui->useNormalsCheckBox->setChecked(m_useNormals);

    changeShader();
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

void MainWindow::changeShader()
{
    DrawableObjectTools::ShaderProgrammType shaderType;
    if (m_useNormals && m_useNormalMap)
    {
        shaderType = DrawableObjectTools::ShaderProgrammType::NormalMap;
    }
    else if (m_useNormals)
    {
        shaderType = DrawableObjectTools::ShaderProgrammType::Lightning;
    }
    else
    {
        shaderType = DrawableObjectTools::ShaderProgrammType::Standard;
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
    QObject::connect(m_openAction, &QAction::triggered, this, &MainWindow::openObjFile);

    // Object's menu actions
    // Change object color
    m_changeObjectColorAction = new QAction(tr("Change object color"), this);
    m_changeObjectColorAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C));
    m_changeObjectColorAction->setToolTip(tr("Use color pallete to change object color"));
    QObject::connect(m_changeObjectColorAction, &QAction::triggered, this, &MainWindow::changeLastObjectColor);

    // Find nearest point index in last object
    m_findNearestPointInLastObjectAction = new QAction(tr("Find nearest point"), this);
    m_findNearestPointInLastObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F));
    m_findNearestPointInLastObjectAction->setToolTip(tr("Use dialog window to input data and find corresponding point in object"));
    QObject::connect(m_findNearestPointInLastObjectAction, &QAction::triggered, this, &MainWindow::findNearestPointInLastObject);

    // Scene's menu actions
    // Delete last object
    m_deleteLastObjectAction = new QAction(tr("Delete last object"), this);
    m_deleteLastObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_X));
    m_deleteLastObjectAction->setToolTip(tr("Delete last object that you added on scene"));
    QObject::connect(m_deleteLastObjectAction, &QAction::triggered, this, &MainWindow::deleteLastObject);

    // Clear objects from scene
    m_clearObjectsAction = new QAction(tr("Clear scene"), this);
    m_clearObjectsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_X));
    m_clearObjectsAction->setToolTip(tr("Clear all objects except "));
    QObject::connect(m_clearObjectsAction, &QAction::triggered, this, &MainWindow::clearObjects);

    // Instrument's menu actions
    // Make target to fit
    m_makeTargetObjectAction = new QAction(tr("Make target object"), this);
    m_makeTargetObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_T));
    m_makeTargetObjectAction->setToolTip(tr("Make target for object to fit"));
    QObject::connect(m_makeTargetObjectAction, &QAction::triggered, this, &MainWindow::makeTargetObject);

    // Perform fitting for current target
    m_performFittingAction = new QAction(tr("Perform fitting for object"), this);
    m_performFittingAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_F));
    m_performFittingAction->setToolTip(tr("Perform fitting for current target object"));
    QObject::connect(m_performFittingAction, &QAction::triggered, this, &MainWindow::performFittingforTarget);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);

    m_objectMenu = menuBar()->addMenu(tr("&Object"));
    m_objectMenu->addAction(m_changeObjectColorAction);
    m_objectMenu->addAction(m_findNearestPointInLastObjectAction);

    m_sceneMenu = menuBar()->addMenu(tr("&Scene"));
    m_sceneMenu->addAction(m_deleteLastObjectAction);
    m_sceneMenu->addAction(m_clearObjectsAction);

    m_instrumentsMenu = menuBar()->addMenu(tr("&Instruments"));
    m_instrumentsMenu->addAction(m_makeTargetObjectAction);
    m_instrumentsMenu->addAction(m_performFittingAction);
}

