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
    fileData = nullptr;

    glWidget = new ObjectViewGLWidget;

    // MenuBar setup:
    createActions();
    createMenus();

    ui->taskProgressBar->setVisible(false);
    ui->taskLabel->setVisible(false);

    glWidget = new ObjectViewGLWidget;

    ui->openGLLayout->addWidget(glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openObjFile()
{
    QString *filePath = new QString(
                QFileDialog::getOpenFileName(this, tr("Choose file"),
                "E:/projects SSD/Qt/3d_project_qmake/res/obj/cube.obj",
                tr("Object (*.obj)")
            ));

    ObjFileReader fileReader = ObjFileReader::Instantiate();

    ui->taskProgressBar->setVisible(true);
    ui->taskLabel->setVisible(true);
    setLabelFontColor(ui->taskLabel, "#3498DB");
    setLabelText(ui->taskLabel, "Reading file: " + *filePath);

    ProgressNotifierSingleton::initialize(ui->taskProgressBar);
    AbstractProgressNotifier* progressNotifier = ProgressNotifierSingleton::getInstance();

    fileData = fileReader.readFile(*filePath, progressNotifier);

    if (fileData == nullptr){
        setLabelFontColor(ui->taskLabel, "#C0392B");
        setLabelText(ui->taskLabel, "Reading failed");

        return;
    } else {
        // If reading was successfull
        setLabelFontColor(ui->taskLabel, "#27AE60");
        setLabelText(ui->taskLabel, "File successfully read");
        setLabelText(ui->objectNameLabel, fileData->getObjectName());
    }

    // Setting file data for glWidget to work with
    glWidget->setFileData(fileData);

    // Memory cleaning
    delete filePath;
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

    QMessageBox* taskMessageBox = new QMessageBox();
    taskMessageBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setLabelFontColor(ui->taskLabel, "#3498DB");
    setLabelText(ui->taskLabel, "Processing Object");

    ProgressNotifierSingleton::initialize(ui->taskProgressBar);
    AbstractProgressNotifier* progressNotifier = ProgressNotifierSingleton::getInstance();

    if (!glWidget->generateArrays(progressNotifier)){
        setLabelFontColor(ui->taskLabel, "#C0392B");
        setLabelText(ui->taskLabel, "Processing failed");

        return;
    }

    setLabelFontColor(ui->taskLabel, "#27AE60");
    setLabelText(ui->taskLabel, "Object successfully processed");

    glWidget->addObject();
    glWidget->update();

    delete taskMessageBox;
}

void MainWindow::setLabelText(QLabel *label, QString text)
{
    label->setText(text);
}

void MainWindow::setLabelFontColor(QLabel *label, QString color)
{
    label->setStyleSheet("{color: " + color + "}");
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

}

void MainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    objectMenu = menuBar()->addMenu(tr("&Object"));
    objectMenu->addAction(showObjectAction);

}

