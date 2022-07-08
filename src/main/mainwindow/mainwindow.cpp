#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <src/widgets/objectviewglwidget/objectviewglwidget.h>

#include <src/service/GlobalState.h>

#include <src/main/colorpicker/colorpicker.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Variables setup
    m_fileData = nullptr;
    m_useNormals = ui->useNormalsCheckBox->isChecked();

    // MenuBar setup:
    createActions();
    createMenus();

    // UI setup
    ui->taskProgressBar->setVisible(false);
    ui->taskLabel->setVisible(false);

    // GlWidget inits
    m_glWidget = new ObjectViewGLWidget;

    ui->openGLLayout->addWidget(m_glWidget);

    // Slots connection
    connect(ui->useNormalsCheckBox, &QCheckBox::toggled, this, &MainWindow::useNormalsCheckBoxClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    ui->taskProgressBar->setVisible(true);
    ui->taskLabel->setVisible(true);
    setLabelFontColor(ui->taskLabel, "yellow");
    setLabelText(ui->taskLabel, "Reading file: " + filePath);

    ProgressNotifierSingleton::initialize(ui->taskProgressBar);
    AbstractProgressNotifier* progressNotifier = ProgressNotifierSingleton::getInstance();
    m_fileData = new ObjReadingTools::ObjFileData();
    QString errorMessage;

    if (!ObjReadingTools::readFile(filePath, *m_fileData, errorMessage, progressNotifier)){
        setLabelFontColor(ui->taskLabel, "red");
        setLabelText(ui->taskLabel, errorMessage);
        qDebug() << errorMessage;
        return;
    } else {
        // If reading was successfull
        setLabelFontColor(ui->taskLabel, "green");
        setLabelText(ui->taskLabel, "File successfully read");
        setLabelText(ui->objectNameLabel, m_fileData->getObjectName());
    }
}

void MainWindow::addObject()
{
    if (m_fileData == nullptr)
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

    setLabelFontColor(ui->taskLabel, "yellow");
    setLabelText(ui->taskLabel, "Processing Object");

    QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(m_fileData->getPolygonVertexIndices());
    QVector<int> polygonNormalIndices = MeshTools::buildPolygonVertexIndicesVector(m_fileData->getPolygonNormalIndices());
    QVector<int> polygonStart = MeshTools::buildPolygonStartVector(m_fileData->getPolygonVertexIndices());

    Object3D *object = new Object3D(
                m_fileData->getVertices(),
                polygonVertexIndices,
                polygonStart,
                m_fileData->getNormals(),
                polygonNormalIndices
                );

    m_glWidget->addObject(object);

    setLabelFontColor(ui->taskLabel, "green");
    setLabelText(ui->taskLabel, "Object added");

    m_glWidget->update();
}

void MainWindow::changeObjectColor()
{
    static ColorPicker *colorPicker = new ColorPicker(this, this, m_glWidget->getObjectColor());
    colorPicker->show();
}

void MainWindow::changeVertexShader()
{
    m_glWidget->setVertexShaderPath(
                QFileDialog::getOpenFileName(
                    this, tr("Choose file"),
                    "../res/shaders/",
                    tr("Vertex shader (*.glsl, *.vsh)")
                    )
                );
}

void MainWindow::changeFragmentShader()
{
    m_glWidget->setFragmentShaderPath(
                QFileDialog::getOpenFileName(
                    this, tr("Choose file"),
                    "../res/shaders/",
                    tr("Fragment shader (*.glsl, *.fsh)")
                    )
                );
}

void MainWindow::useNormalsCheckBoxClicked(bool checked)
{
    m_useNormals = checked;
    changeShader();
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
    shaderType = m_useNormals ?
                DrawableObjectTools::ShaderProgrammType::Lightning
                              :
                DrawableObjectTools::ShaderProgrammType::Standard;

    m_glWidget->switchShaders(shaderType);
    m_glWidget->update();
}

void MainWindow::createActions()
{
    // Files actions
    // Open file Action
    m_openAction = new QAction(tr("Open file"), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open a new file"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openObjFile);

    // Objects actions
    // Show object action
    m_addObjectAction = new QAction(tr("Add object"), this);
    m_addObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    m_addObjectAction->setStatusTip(tr("Add object that you got from a file to a scene"));
    connect(m_addObjectAction, &QAction::triggered, this, &MainWindow::addObject);

    // Change object color
    m_changeObjectColorAction = new QAction(tr("Change object color"), this);
    m_changeObjectColorAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C));
    m_changeObjectColorAction->setToolTip(tr("Use color pallete to change object color"));
    connect(m_changeObjectColorAction, &QAction::triggered, this, &MainWindow::changeObjectColor);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);

    m_objectMenu = menuBar()->addMenu(tr("&Object"));
    m_objectMenu->addAction(m_addObjectAction);
    m_objectMenu->addAction(m_changeObjectColorAction);
}

