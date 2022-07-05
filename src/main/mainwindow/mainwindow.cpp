#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <src/widgets/objectviewglwidget/objectviewglwidget.h>

#include <src/service/GlobalState.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // UI Style setup
    this->setStyleSheet("background-color: white;");

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
    connect(ui->useNormalsCheckBox, SIGNAL(QCheckBox::clicked(bool)), this, SLOT(useNormalsCheckBoxClicked(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openObjFile()
{
    QString filePath(
                QFileDialog::getOpenFileName(this, tr("Choose file"),
                "E:/projects SSD/Qt/3d_project_qmake/res/obj/cube.obj",
                tr("Object (*.obj)")
            ));

    ObjFileReader fileReader = ObjFileReader::Instantiate();

    ui->taskProgressBar->setVisible(true);
    ui->taskLabel->setVisible(true);
    setLabelFontColor(ui->taskLabel, "yellow");
    setLabelText(ui->taskLabel, "Reading file: " + filePath);

    ProgressNotifierSingleton::initialize(ui->taskProgressBar);
    AbstractProgressNotifier* progressNotifier = ProgressNotifierSingleton::getInstance();

    fileData = new ObjFileData();
    fileReader.readFile(filePath, *fileData, progressNotifier);

    if (fileData == nullptr){
        setLabelFontColor(ui->taskLabel, "red");
        setLabelText(ui->taskLabel, "Reading failed");

        return;
    } else {
        // If reading was successfull
        setLabelFontColor(ui->taskLabel, "green");
        setLabelText(ui->taskLabel, "File successfully read");
        setLabelText(ui->objectNameLabel, fileData->getObjectName());
    }
}

void MainWindow::showObject()
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

    ProgressNotifierSingleton::initialize(ui->taskProgressBar);
    AbstractProgressNotifier* progressNotifier = ProgressNotifierSingleton::getInstance();

    if (!glWidget->generateArrays(*fileData, progressNotifier)){
        setLabelFontColor(ui->taskLabel, "red");
        setLabelText(ui->taskLabel, "Processing failed");

        return;
    }

    setLabelFontColor(ui->taskLabel, "green");
    setLabelText(ui->taskLabel, "Object successfully processed");

    glWidget->addObject();
    glWidget->update();
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

void MainWindow::reinitShaderProgram()
{
    glWidget->reinit();
    glWidget->update();
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

    // Open file Action
    openAction = new QAction(tr("Open file"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a new file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openObjFile);

    // Show object action
    showObjectAction = new QAction(tr("Show object"), this);
    // TODO:: rework with QKeyCombination class
    showObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    showObjectAction->setStatusTip(tr("Show object that you got from a file"));
    connect(showObjectAction, &QAction::triggered, this, &MainWindow::showObject);

    changeVertexShaderAction = new QAction(tr("Change vertex shader"), this);
    changeVertexShaderAction->setShortcut(QKeySequence(Qt::Key_S | Qt::Key_C | Qt::Key_V));
    changeVertexShaderAction->setStatusTip(tr("Change vertex shader that OpenGL uses to display objects"));
    connect(changeVertexShaderAction, &QAction::triggered, this, &MainWindow::changeVertexShader);

    changeFragmentShaderAction = new QAction(tr("Change fragment shader"), this);
    changeFragmentShaderAction->setShortcut(QKeySequence(Qt::Key_S | Qt::Key_C | Qt::Key_V));
    changeFragmentShaderAction->setStatusTip(tr("Change fragment shader that OpenGL uses to display objects"));
    connect(changeFragmentShaderAction, &QAction::triggered, this, &MainWindow::changeFragmentShader);

    reinitShaderProgramAction = new QAction(tr("Reinit widget"), this);
    reinitShaderProgramAction->setShortcut(QKeySequence(Qt::Key_S | Qt::Key_R));
    reinitShaderProgramAction->setStatusTip(tr("Reinit shader program after setting other shaders"));
    connect(reinitShaderProgramAction, &QAction::triggered, this, &MainWindow::reinitShaderProgram);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    objectMenu = menuBar()->addMenu(tr("&Object"));
    objectMenu->addAction(showObjectAction);

    shaderMenu = menuBar()->addMenu(tr("&Shader"));
    shaderMenu->addAction(changeVertexShaderAction);
    shaderMenu->addAction(changeFragmentShaderAction);
    shaderMenu->addAction(reinitShaderProgramAction);
}

