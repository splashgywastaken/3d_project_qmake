#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QThread>
#include "qlayout.h"
#include <QtConcurrent/QtConcurrent>

#include <src/service/GlobalState.h>
#include <src/main/FindNearestPointDialog/findnearestpointdialog.h>

#include <src/service/Optimization/OptimizationUtils/optimizationutils.h>
#include <src/service/Optimization/LambdaStepCallback/lambdastepcallback.h>
#include <src/service/Optimization/GradientDescent/gradientdescent.h>

#include <src/service/RigidAlignment/RigidAlignmentProblem/rigidalignmentproblem.h>
#include <src/service/RigidAlignment/RigidAlignmentScalingProblem/rigidalignmentscalingproblem.h>
#include <src/service/RigidAlignment/ClosestPointsBasedAlignmentProblem/closestpointsbasedalignmentproblem.h>

#include <src/service/GeomTools/GeomTools.h>
#include <src/service/QVectorTools/QVectorTools.h>

#include <src/service/Registration/RegistrationClasses/Registration/registration.h>
#include <src/service/Registration/ClosestPointsFinderKDTree/closestpointsfinderkdtree.h>
#include <src/service/Registration/FitterClasses/LambdaStepCallback/lambdastepcallback.h>
#include <src/service/Registration/FitterClasses/RigidFitter/rigidfitter.h>

#include "src/service/KDTree/kdtree.h"
#include <src/widgets/objectviewglwidget/objectviewglwidget.h>

const QColor defaultSceneObjectColor(85, 170, 255);
const QColor defaultTargetObjectColor(255, 0, 0);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    // Style sheet setup:
    QFile styleFile(":/main/mainwindow/mainwindow.qss");
    styleFile.open(QFile::ReadOnly);
    this->setStyleSheet(QString::fromLatin1( styleFile.readAll() ));
    styleFile.close();

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

void MainWindow::loadObjectFromObjFile(bool value)
{
    Q_UNUSED(value);

    m_objDataBase = new ObjReadingTools::ObjFileData();
    QString errorMessage;
    if (readObjectFromFile(*m_objDataBase, tr("Choose file"), tr("Object (*.obj)"), errorMessage, QDir::homePath()))
    {
        addSceneObjectFromObjData(
                    *m_objDataBase,
                    m_baseSceneObject,
                    defaultSceneObjectColor,
                    m_glWidget
                    );
        delete m_currentObjectColor;
        m_currentObjectColor = new QColor(defaultSceneObjectColor);
        return;
    }
    if (!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Failed to load mesh"), errorMessage);
    }
}

void MainWindow::deleteLastObject(bool value)
{
    Q_UNUSED(value);

    m_glWidget->deleteLastObject();
    m_glWidget->update();
    m_glWidget->setObjectColor(m_glWidget->getObjectColor());
}

void MainWindow::clearObjects(bool value)
{
    Q_UNUSED(value);

    // Base objects
    delete m_baseSceneObject;
    m_baseSceneObject = nullptr;
    // Base obj data
    delete m_objDataBase;
    m_objDataBase = nullptr;

    // Fitting objects
    delete m_fittingBaseSceneObject;
    m_fittingBaseSceneObject = nullptr;
    delete m_fittingTargetSceneObject;
    m_fittingTargetSceneObject = nullptr;
    // Fitting data
    delete m_fittingObjDataBase;
    m_fittingObjDataBase = nullptr;
    delete m_fittingObjDataTarget;
    m_fittingObjDataTarget = nullptr;
    delete m_fittingObjDataResult;
    m_fittingObjDataResult = nullptr;

    // Registration scene objects
    delete m_registrationBaseMeshSceneObject;
    m_registrationBaseMeshSceneObject = nullptr;
    delete m_registrationTargetMeshSceneObject;
    m_registrationTargetMeshSceneObject = nullptr;
    // Registration obj data
    delete m_registrationObjDataBase;
    m_registrationObjDataBase = nullptr;
    delete m_registrationObjDataTarget;
    m_registrationObjDataTarget = nullptr;
    delete m_registrationObjDataResult;
    m_registrationObjDataResult = nullptr;

    m_glWidget->clearObjects();
    m_glWidget->update();
    m_glWidget->setObjectColor(m_glWidget->getObjectColor());
}

void MainWindow::changeLastObjectColor(bool value)
{
    Q_UNUSED(value);

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

void MainWindow::findNearestPointInLastObject(bool value)
{
    Q_UNUSED(value);

    if (m_objDataBase == nullptr)
    {
        QMessageBox::warning(
                    this,
                    tr("Failed to find an object"),
                    tr("Load object first to find nearest point in")
                    );
        return;
    }

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

void MainWindow::loadFittingBaseObject(bool value)
{
    Q_UNUSED(value);

    m_fittingObjDataBase = new ObjReadingTools::ObjFileData();
    QString errorMessage;
    if (!readObjectFromFile(*m_fittingObjDataBase, tr("Choose base mesh file"), tr("Object (*.obj)"), errorMessage, QDir::homePath()))
    {
        if (!errorMessage.isEmpty())
        {
            QMessageBox::warning(
                        this,
                        tr("Failed to load mesh"),
                        errorMessage
                        );
        }
        return;
    }

    addSceneObjectFromObjData(
                *m_fittingObjDataBase,
                m_fittingBaseSceneObject,
                defaultSceneObjectColor,
                m_glWidget
                );
    m_currentObjectColor = new QColor(defaultSceneObjectColor);
}

void MainWindow::makeFittingTargetObject(bool value)
{
    Q_UNUSED(value);

    if (m_fittingBaseSceneObject == nullptr)
    {
        QMessageBox::warning(
                    this,
                    tr("Failed to make target"),
                    tr("Base object was not loaded.")
                    );
        return;
    }

    QMatrix4x4 hardcodedTransformation;
    hardcodedTransformation.setToIdentity();

    hardcodedTransformation.scale(1);

    hardcodedTransformation.translate(2.0f, 0.0f, 2.0f);

    hardcodedTransformation.rotate(90, QVector3D(1, 0, 0));
    hardcodedTransformation.rotate(90, QVector3D(0, 0, 1));

    if (m_fittingObjDataTarget == nullptr)
    {
        delete m_fittingObjDataTarget;
        m_fittingObjDataTarget = nullptr;
    }
    m_fittingObjDataTarget = new ObjReadingTools::ObjFileData();
    *m_fittingObjDataTarget = *m_fittingObjDataBase;

    int size = m_fittingObjDataTarget->getVertices().size();
    QVector<QVector3D> targetVertices = (m_fittingObjDataTarget->getVertices());
    for (int index = 0; index < size; index++)
    {
       targetVertices[index] = hardcodedTransformation.map(targetVertices[index]);
    }
    m_fittingObjDataTarget->setVertices(targetVertices);

    if (m_fittingTargetSceneObject != nullptr)
    {
        m_glWidget->removeObject(m_fittingTargetSceneObject);
        delete m_fittingTargetSceneObject;
        m_fittingTargetSceneObject = nullptr;
    }
    addSceneObjectFromObjData(
                *m_fittingObjDataTarget,
                m_fittingTargetSceneObject,
                defaultTargetObjectColor,
                m_glWidget
                );
}

void MainWindow::performFittingforTarget(bool value)
{
    Q_UNUSED(value);

    if (m_fittingBaseSceneObject == nullptr || m_fittingTargetSceneObject == nullptr)
    {
        QMessageBox::warning(
                    this,
                    tr("Failed to make fitting"),
                    tr("Either base object was not loaded or there is no target object."
                       "\nPlease load base object and make target in order to perform fitting.")
                    );
        return;
    }

    if (m_fittingObjDataResult != nullptr)
    {
        delete m_fittingObjDataResult;
        m_fittingObjDataResult = nullptr;
    }
    m_fittingObjDataResult = new ObjReadingTools::ObjFileData();

    const auto stepFunction = [&](const QVector<double>& variables)
    {
        *m_fittingObjDataResult = *m_fittingObjDataBase;
        const QMatrix4x4 transformation = Optimization::RigidAlignmentScalingProblem::transformationMatrixFromVars(variables);
        const int resultSize = m_fittingObjDataResult->getVertices().size();
        QVector<QVector3D> resultVertices = m_fittingObjDataResult->getVertices().toVector();
        for (int index = 0; index < resultSize; index++)
        {
            resultVertices[index] = transformation.map(resultVertices[index]);
        }
        m_fittingObjDataResult->setVertices(resultVertices);

        addSceneObjectFromObjData(
                    *m_fittingObjDataResult,
                    m_fittingBaseSceneObject,
                    *m_currentObjectColor,
                    m_glWidget
                    );
        QApplication::processEvents();
        QThread::msleep(5);
    };

    Optimization::Problem *problem = new Optimization::RigidAlignmentScalingProblem(
                m_fittingObjDataBase->getVertices(),
                m_fittingObjDataTarget->getVertices()
                );
    Optimization::LambdaStepCallback callback(stepFunction);

    const QVector<double> initialVariables = {0, 0, 0, 0, 0, 0, 1};
    const double stepLength = 0.4;
    const int nMaxIterations = 1500;
    const double gradientNormThreshold = 9e-7;

    QVector<double> result = Optimization::gradientDescent(
                problem, initialVariables,
                stepLength, nMaxIterations, gradientNormThreshold,
                true, &callback
                );

    result = Optimization::RigidAlignmentScalingProblem::transformationVectorFromVars(result);

    QMessageBox yesNoMessageBox(this);
    setupYesNoTransformMessageBox(
                yesNoMessageBox,
                tr("Fitting is finished"),
                tr("Do you want to remove target object?"),
                result
                );

    if (yesNoMessageBox.exec() == QMessageBox::Yes)
    {
        m_glWidget->removeObject(m_fittingTargetSceneObject);
        delete m_fittingTargetSceneObject;
        m_fittingTargetSceneObject = nullptr;
    }
}

void MainWindow::loadRegistrationBaseMeshObject(bool value)
{
    Q_UNUSED(value);

    m_registrationObjDataBase = new ObjReadingTools::ObjFileData();
    QString errorMessage;
    if (!readObjectFromFile(*m_registrationObjDataBase, tr("Choose base mesh file"), tr("Object (*.obj)"), errorMessage, QDir::homePath()))
    {
        if (!errorMessage.isEmpty())
        {
            QMessageBox::warning(
                        this,
                        tr("Failed to load mesh"),
                        errorMessage
                        );
        }
        return;
    }
    bool hasNormals =
            !m_registrationObjDataBase->getNormals().isEmpty() &&
            !m_registrationObjDataBase->getPolygonNormalIndices().isEmpty();
    if (!hasNormals)
    {
        addNormalsToObjData(*m_registrationObjDataBase);
    }

    addSceneObjectFromObjData(
                *m_registrationObjDataBase,
                m_registrationBaseMeshSceneObject,
                defaultSceneObjectColor,
                m_glWidget
                );
    m_currentObjectColor = new QColor(defaultSceneObjectColor);
}

void MainWindow::performClosestPointsBasedFitting(bool value)
{
    Q_UNUSED(value);

    if (m_fittingBaseSceneObject == nullptr || m_fittingTargetSceneObject == nullptr)
    {
        QMessageBox::warning(
                    this,
                    tr("Failed to make fitting"),
                    tr("Either base object was not loaded or there is no target object."
                       "\nPlease load base object and make target in order to perform fitting.")
                    );
        return;
    }

    if (m_fittingObjDataResult != nullptr)
    {
        delete m_fittingObjDataResult;
        m_fittingObjDataResult = nullptr;
    }
    m_fittingObjDataResult = new ObjReadingTools::ObjFileData();

    const auto stepFunction = [&](const QVector<double>& variables)
    {
        *m_fittingObjDataResult = *m_fittingObjDataBase;
        const QMatrix4x4 transformation = Optimization::ClosestPointsBasedAlignmentProblem::transformationMatrixFromVariables(variables);
        const int resultSize = m_fittingObjDataResult->getVertices().size();
        QVector<QVector3D> resultVertices = m_fittingObjDataResult->getVertices().toVector();
        for (int index = 0; index < resultSize; index++)
        {
            resultVertices[index] = transformation.map(resultVertices[index]);
        }
        m_fittingObjDataResult->setVertices(resultVertices);

        addSceneObjectFromObjData(
                    *m_fittingObjDataResult,
                    m_fittingBaseSceneObject,
                    *m_currentObjectColor,
                    m_glWidget
                    );
        QApplication::processEvents();
        QThread::msleep(5);
    };

    Registration::ClosestPointsFinderKDTree closestPointsFinder(m_fittingObjDataTarget->getVertices());

    const QVector<int> basePolygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(m_fittingObjDataBase->getPolygonVertexIndices());
    const QVector<int> basePolygonStart = MeshTools::buildPolygonStartVector(m_fittingObjDataBase->getPolygonVertexIndices());
    const QVector<int> baseTriangleVertexIndices = MeshTools::buildTriangleVertexIndices(basePolygonVertexIndices, basePolygonStart);
    const QVector<QVector3D> baseVertices = m_fittingObjDataBase->getVertices();
    const QVector<QVector3D> targetVertices = m_fittingObjDataTarget->getVertices();
    const QVector<QVector3D> targetNormals = m_fittingObjDataTarget->getNormals();

    Optimization::Problem* problem = new Optimization::ClosestPointsBasedAlignmentProblem(
                baseTriangleVertexIndices,
                baseVertices,
                targetVertices,
                targetNormals,
                closestPointsFinder
                );
    Optimization::LambdaStepCallback callback(stepFunction);

    const QVector<double> initialVariables = {0, 0, 0, 0, 0, 0};
    const double stepLength = 0.4;
    const int nMaxIterations = 1500;
    const double gradientNormThreshold = 9e-7;
    const int nLineSearchIterations = 10;
    const double stepLengthMax = 10;
    const bool verbose = true;

    QVector<double> result = Optimization::gradientDescentWithBackTrackingLineSearch(
                *problem, initialVariables,
                stepLength, nMaxIterations, gradientNormThreshold, nLineSearchIterations, stepLengthMax, verbose,
                &callback
                );

    QMessageBox yesNoMessageBox(this);
    setupYesNoTransformMessageBox(
                yesNoMessageBox,
                tr("Fitting is finished"),
                tr("Do you want to remove target object?"),
                result
                );

    if (yesNoMessageBox.exec() == QMessageBox::Yes)
    {
        m_glWidget->removeObject(m_fittingTargetSceneObject);
        delete m_fittingTargetSceneObject;
        m_fittingTargetSceneObject = nullptr;
    }
}

void MainWindow::loadRegistrationTargetMeshObject(bool value)
{
    Q_UNUSED(value);

    m_registrationObjDataTarget = new ObjReadingTools::ObjFileData();
    QString errorMessage;
    if (!readObjectFromFile(*m_registrationObjDataTarget, tr("Choose target mesh file"), tr("Object (*.obj)"), errorMessage, QDir::homePath()))
    {
        if (!errorMessage.isEmpty())
        {
            QMessageBox::warning(
                        this,
                        tr("Failed to load mesh"),
                        errorMessage
                        );
        }
        return;
    }
    bool hasNormals =
            !m_registrationObjDataTarget->getNormals().isEmpty() &&
            !m_registrationObjDataTarget->getPolygonNormalIndices().isEmpty();
    if (!hasNormals)
    {
        addNormalsToObjData(*m_registrationObjDataTarget);
    }

    addSceneObjectFromObjData(
                *m_registrationObjDataTarget,
                m_registrationTargetMeshSceneObject,
                defaultTargetObjectColor,
                m_glWidget
                );
}

void MainWindow::performRigidRegistration(bool value)
{
    Q_UNUSED(value);

    if (m_registrationBaseMeshSceneObject == nullptr || m_registrationTargetMeshSceneObject == nullptr)
    {
        QMessageBox::warning(
                    this,
                    tr("Failed to make resgitration"),
                    tr("Either base object was not loaded or there is no target object."
                       "\nPlease load base object and make target in order to perform registration.")
                    );
        return;
    }
    m_registrationObjDataResult = new ObjReadingTools::ObjFileData();
    *m_registrationObjDataResult = *m_registrationObjDataBase;

    const double optimalStepLength = 0.02;
    const int optimalNMaxSteps = 10;
    const int nIcpIterations = 75;
    const float minCosBetweenNormals = 0.3f;

    const auto stepFunction = [&](const QVector<QVector3D> &newVertices)
    {
        m_registrationObjDataResult->setVertices(newVertices);
        addSceneObjectFromObjData(
                    *m_registrationObjDataResult,
                    m_registrationBaseMeshSceneObject,
                    *m_currentObjectColor,
                    m_glWidget
                    );

        QApplication::processEvents();
        QThread::msleep(5);
    };

    Registration::LambdaStepCallback callback(stepFunction);

    Registration::RigidFitter fitter(
                m_registrationObjDataBase->getVertices(),
                optimalStepLength, optimalNMaxSteps
                );
    Registration::ClosestPointsFinderKDTree closestPointsFinder(m_registrationObjDataTarget->getVertices());

    const QVector<int> basePolygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(m_registrationObjDataBase->getPolygonVertexIndices());
    const QVector<int> basePolygonStart = MeshTools::buildPolygonStartVector(m_registrationObjDataBase->getPolygonVertexIndices());
    const QVector<int> baseTriangleVertexIndices = MeshTools::buildTriangleVertexIndices(basePolygonVertexIndices, basePolygonStart);
    const QVector<QVector3D> baseVertices = m_registrationObjDataBase->getVertices();
    const QVector<QVector3D> targetVertices = m_registrationObjDataTarget->getVertices();
    const QVector<QVector3D> targetNormals = m_registrationObjDataTarget->getNormals();

    Registration::performIcp(
                fitter, closestPointsFinder,
                baseTriangleVertexIndices, baseVertices, targetVertices, targetNormals,
                nIcpIterations, minCosBetweenNormals, true, &callback);

    QMessageBox yesNoMessageBox(this);
    setupYesNoTransformMessageBox(
                yesNoMessageBox,
                tr("Registration is finished"),
                tr("Do yo want to remove target object?")
                );

    if (yesNoMessageBox.exec() == QMessageBox::Yes)
    {
        m_glWidget->removeObject(m_registrationTargetMeshSceneObject);
        delete m_registrationTargetMeshSceneObject;
        m_registrationTargetMeshSceneObject = nullptr;
    }
}

void MainWindow::changeBackgroundColor(bool value)
{
    Q_UNUSED(value);

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

void MainWindow::changeGridColor(bool value)
{
    Q_UNUSED(value);

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
        QString& errorMessage,
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
    if (!ObjReadingTools::readFile(resultFilePath, destObj, errorMessage, progressNotifier)){
        delete fileDialog;
        #ifdef QT_DEBUG
            qDebug() << errorMessage;
        #endif
        return false;
    }

    delete fileDialog;
    return true;
}

void MainWindow::addSceneObjectFromObjData(
        ObjReadingTools::ObjFileData& obj,
        SceneObject*& object,
        const QColor& color,
        ObjectViewGLWidget* glWidget
        )
{
    QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(obj.getPolygonVertexIndices());
    QVector<int> polygonNormalIndices = MeshTools::buildPolygonVertexIndicesVector(obj.getPolygonNormalIndices());
    QVector<int> polygonStart = MeshTools::buildPolygonStartVector(obj.getPolygonVertexIndices());
    glWidget->makeCurrent();
    if (object != nullptr )
    {
        glWidget->removeObject(object);
        delete object;
    }
    object = new Object3D(
                obj.getVertices(),
                polygonVertexIndices,
                polygonStart,
                obj.getNormals(),
                polygonNormalIndices
                );
    glWidget->addObject(object);
    glWidget->setObjectColor(color);
    glWidget->update();
}

void MainWindow::addNormalsToObjData(ObjReadingTools::ObjFileData &objData)
{
    const QVector<int> basePolygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(objData.getPolygonVertexIndices());
    const QVector<int> basePolygonStart = MeshTools::buildPolygonStartVector(objData.getPolygonVertexIndices());
    const QVector<int> baseTriangleVertexIndices = MeshTools::buildTriangleVertexIndices(basePolygonVertexIndices, basePolygonStart);
    objData.setNormals(GeomTools::computeNormals(baseTriangleVertexIndices, objData.getVertices()));
    objData.setPolygonNormalIndices(objData.getPolygonVertexIndices());
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
    // File menu actions
    // Open file Action
    setupAction(
                m_loadObjectFromObjFileAction,
                this,
                tr("Open file"),
                QKeySequence::Open,
                tr("Open a new file"),
                this,
                SLOT(loadObjectFromObjFile(bool))
                );

    // Object menu actions
    // Change object color
    setupAction(
                m_changeObjectColorAction,
                this,
                tr("Change object color"),
                QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C),
                tr("Use color pallete to change object color"),
                this,
                SLOT(changeLastObjectColor(bool))
                );

    // Find nearest point index in last object
    setupAction(
                m_findNearestPointInLastObjectAction,
                this,
                tr("Find nearest point"),
                QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F),
                tr("Use dialog window to input data and find corresponding point in object"),
                this,
                SLOT(findNearestPointInLastObject(bool))
                );

    // Scene menu actions
    // Delete last object
    setupAction(
                m_deleteLastObjectAction,
                this,
                tr("Delete last object"),
                QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_X),
                tr("Delete last object that you added on scene"),
                this,
                SLOT(deleteLastObject(bool))
                );

    // Clear objects from scene
    setupAction(
                m_clearObjectsAction,
                this,
                tr("Clear scene"),
                QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_X),
                tr("Clear all objects except "),
                this,
                SLOT(clearObjects(bool))
                );

    // Fitting menu actions
    // Load base object to make target and perform fitting
    setupAction(
                m_loadFittingBaseObjectActon,
                this,
                tr("Load base object"),
                QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_B),
                tr("Load base object to make target and perform fitting"),
                this,
                SLOT(loadFittingBaseObject(bool))
                );

    // Make target to fit
    setupAction(
                m_makeFittingTargetObjectAction,
                this,
                tr("Make target object"),
                QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_T),
                tr("Make target for object to fit"),
                this,
                SLOT(makeFittingTargetObject(bool))
                );

    // Perform fitting for current target
    setupAction(
                m_performFittingAction,
                this,
                tr("Perform fitting for object"),
                QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_F),
                tr("Perform fitting for current target object"),
                this,
                SLOT(performFittingforTarget(bool))
                );

    // Perform fitting with use of closest points in model instead of regular fitting with all points
    setupAction(
                m_performClosestPointsBasedFitting,
                this,
                tr("Perform fitting based on closest points"),
                QKeyCombination(Qt::ControlModifier | Qt::AltModifier, Qt::Key_R),
                tr("Perform closest points based fitting for base and target objects."),
                this,
                SLOT(performClosestPointsBasedFitting(bool))
                );

    // Registration menu actions
    // Load base mesh to a scene
    setupAction(
                m_loadRegistrationBaseMeshObjectAction,
                this,
                tr("Load base mesh"),
                QKeyCombination(Qt::ControlModifier | Qt::AltModifier, Qt::Key_B),
                tr("Load an object for base mesh for registration from .obj file"),
                this,
                SLOT(loadRegistrationBaseMeshObject(bool))
                );
    // Load target mesh to a scene
    setupAction(
                m_loadRegistrationTargetObjectAction,
                this,
                tr("Load target mesh"),
                QKeyCombination(Qt::ControlModifier | Qt::AltModifier, Qt::Key_T),
                tr("Load an object for target mesh for registration from .obj file"),
                this,
                SLOT(loadRegistrationTargetMeshObject(bool))
                );
    // Perform resgitration
    setupAction(
                m_performRigidRegistrationAction,
                this,
                tr("Perform rigid registration"),
                QKeyCombination(Qt::ControlModifier | Qt::AltModifier, Qt::Key_R),
                tr("Perform rigid registration for BaseMesh and Target objects."),
                this,
                SLOT(performRigidRegistration(bool))
                );

    // View menu actions
    // Change background's color
    setupAction(
                m_changeBackgroundColorAction,
                this,
                tr("Change background color"),
                QKeySequence(),
                tr("Change background color with color picker dialog"),
                this,
                SLOT(changeBackgroundColor(bool))
                );

    setupAction(
                m_changeGridColorAction,
                this,
                tr("Change grid color"),
                QKeySequence(),
                tr("Change grid color with color picker dialog"),
                this,
                SLOT(changeGridColor(bool))
                );
}

void MainWindow::setupAction(
        QAction*& action,
        QObject* actionsParent,
        const QString& caption,
        const QKeySequence& keySequence,
        const QString& toolTip,
        QObject* connectionReciever,
        const char* connectionSlot
        )
{
    action = new QAction(caption, actionsParent);
    action->setShortcut(keySequence);
    action->setStatusTip(toolTip);
    connect(action, SIGNAL(triggered(bool)), connectionReciever, connectionSlot);
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
    setupMenu(
                m_fileMenu,
                tr("File"),
                tr("Files related stuff"),
                { m_loadObjectFromObjFileAction }
                );

    setupMenu(
                m_objectMenu,
                tr("Object"),
                tr("Objects related stuff"),
                { m_changeObjectColorAction, m_findNearestPointInLastObjectAction }
                );

    // Shader's menu actions
    m_switchShaderMenu = new QMenu(tr("Change shader"), this);
    m_switchShaderMenu->setToolTip(tr("Use list of shaders to switch your current one"));
    m_shaderMenu = menuBar()->addMenu(tr("&Shaders"));
    m_shaderMenu->addMenu(m_switchShaderMenu);

    setupMenu(
                m_sceneMenu,
                tr("Scene"),
                tr("Clear and delete objects"),
                { m_deleteLastObjectAction, m_clearObjectsAction }
                );

    setupMenu(
                m_fittingMenu,
                tr("Fitting"),
                tr("Make target and perform fitting"),
                {
                    m_loadFittingBaseObjectActon,
                    m_makeFittingTargetObjectAction,
                    m_performFittingAction,
                    m_performClosestPointsBasedFitting
                }
                );

    setupMenu(
                m_registrationMenu,
                tr("Registration"),
                tr("Load base and target meshes, perform ICP registration"),
                {
                    m_loadRegistrationBaseMeshObjectAction,
                    m_loadRegistrationTargetObjectAction,
                    m_performRigidRegistrationAction
                });

    setupMenu(
                m_viewMenu,
                tr("View"),
                tr("Change colors of scene's background and grid"),
                { m_changeBackgroundColorAction, m_changeGridColorAction }
                );
}

void MainWindow::setupMenu(
        QMenu *&menu,
        const QString &caption,
        const QString &toolTip,
        QList<QAction *> actionsList
        )
{
    menu = menuBar()->addMenu(caption);
    menu->setToolTip(toolTip);
    if (!actionsList.empty())
    {
        int nActions = actionsList.size();

        for (int index = 0; index < nActions; index++)
        {
            menu->addAction(actionsList[index]);
        }
    }
}

void MainWindow::setupYesNoTransformMessageBox(
        QMessageBox& yesNoMessageBox,
        QString windowTitle,
        QString mainText,
        QVector<double> transformationVector
        )
{
    yesNoMessageBox.setWindowTitle(windowTitle);
    yesNoMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (!transformationVector.isEmpty())
    {
        yesNoMessageBox.setText(
                    QString(tr("Estimated transformation:\nTranslation (%1, %2, %3)\nRotation (%4, %5, %6)"))
                    .arg(transformationVector[0]).arg(transformationVector[1]).arg(transformationVector[2])
                .arg(transformationVector[3]).arg(transformationVector[4]).arg(transformationVector[5])
                );
        if (transformationVector.size() == 7)
        {
            yesNoMessageBox.setText(yesNoMessageBox.text() + QString("\nScaling %7")
                                    .arg(transformationVector[6]));
        }
        else if (transformationVector.size() == 9)
        {
            yesNoMessageBox.setText(yesNoMessageBox.text() + QString("\nScaling (%7, %8, %9)")
                                    .arg(transformationVector[6]).arg(transformationVector[7]).arg(transformationVector[8]));
        }
    }
    yesNoMessageBox.setText(yesNoMessageBox.text() + "\n" + mainText);
}
