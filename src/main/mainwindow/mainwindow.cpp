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
    fileData = nullptr;

    // MenuBar setup:
    createActions();
    createMenus();

    // UI setup
    ui->taskProgressBar->setVisible(false);
    ui->taskLabel->setVisible(false);

    // GlWidget inits
    glWidget = new ObjectViewGLWidget;

    ui->openGLLayout->addWidget(glWidget);

    // Slots connection
    connect(ui->useNormalsCheckBox, &QCheckBox::toggled, this, &MainWindow::useNormalsCheckBoxClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setObjectColor(QVector3D objectColor)
{
    glWidget->setObjectColor(objectColor);
    glWidget->update();
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
    fileData = new ObjReadingTools::ObjFileData();
    QString errorMessage;

    if (!ObjReadingTools::readFile(filePath, *fileData, errorMessage, progressNotifier)){
        setLabelFontColor(ui->taskLabel, "red");
        setLabelText(ui->taskLabel, errorMessage);
        qDebug() << errorMessage;
        return;
    } else {
        // If reading was successfull
        setLabelFontColor(ui->taskLabel, "green");
        setLabelText(ui->taskLabel, "File successfully read");
        setLabelText(ui->objectNameLabel, fileData->getObjectName());
    }
}

void MainWindow::addObject()
{
    if (fileData == nullptr)
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

    QVector<int> polygonVertexIndices = MeshTools::buildPolygonVertexIndicesVector(fileData->getPolygonVertexIndices());
    QVector<int> polygonNormalIndices = MeshTools::buildPolygonVertexIndicesVector(fileData->getPolygonNormalIndices());
    QVector<int> polygonStart = MeshTools::buildPolygonStartVector(fileData->getPolygonVertexIndices());

    Object3D *object = new Object3D(
                fileData->getVertices(),
                polygonVertexIndices,
                polygonStart,
                fileData->getNormals(),
                polygonNormalIndices
                );

    glWidget->addObject(object);

    setLabelFontColor(ui->taskLabel, "green");
    setLabelText(ui->taskLabel, "Object added");

    glWidget->update();
}

void MainWindow::changeObjectColor()
{
    static ColorPicker *colorPicker = new ColorPicker(this, this, glWidget->getObjectColor());
    colorPicker->show();
}

void MainWindow::changeVertexShader()
{
    glWidget->setVertexShaderPath(
                QFileDialog::getOpenFileName(
                    this, tr("Choose file"),
                    "../res/shaders/",
                    tr("Vertex shader (*.glsl, *.vsh)")
                    )
                );
}

void MainWindow::changeFragmentShader()
{
    glWidget->setFragmentShaderPath(
                QFileDialog::getOpenFileName(
                    this, tr("Choose file"),
                    "../res/shaders/",
                    tr("Fragment shader (*.glsl, *.fsh)")
                    )
                );
}

void MainWindow::useNormalsCheckBoxClicked(bool checked)
{
    glWidget->setUseNormals(checked);
}

void MainWindow::setLabelText(QLabel *label, QString text)
{
    label->setText(text);
}

void MainWindow::setLabelFontColor(QLabel *label, QString color)
{
    label->setStyleSheet("color: " + color);
}

void MainWindow::createActions()
{
    // Files actions
    // Open file Action
    openAction = new QAction(tr("Open file"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a new file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openObjFile);

    // Objects actions
    // Show object action
    addObjectAction = new QAction(tr("Add object"), this);
    addObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    addObjectAction->setStatusTip(tr("Add object that you got from a file to a scene"));
    connect(addObjectAction, &QAction::triggered, this, &MainWindow::addObject);

    // Change object color
    changeObjectColorAction = new QAction(tr("Change object color"), this);
    changeObjectColorAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C));
    changeObjectColorAction->setToolTip(tr("Use color pallete to change object color"));
    connect(changeObjectColorAction, &QAction::triggered, this, &MainWindow::changeObjectColor);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    objectMenu = menuBar()->addMenu(tr("&Object"));
    objectMenu->addAction(addObjectAction);
    objectMenu->addAction(changeObjectColorAction);
}

