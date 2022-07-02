#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <src/service/file_readers/ObjFileReader/ObjFileReader.h>
#include <src/widgets/customglwidget/customglwidget.h>
#include <src/widgets/objectviewglwidget/objectviewglwidget.h>
//#include <src/widgets/customglwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileData = nullptr;

    glWidget = new ObjectViewGLWidget;
    //glWidget = new CustomGLWidget;
    //ui->openGLLayout->addWidget(glWidget);

    // MenuBar setup:
    createActions();
    createMenus();

    ui->taskProgressBar->setVisible(false);
    ui->taskLabel->setVisible(false);
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
    setLabelText(ui->taskLabel, "Reading file: " + *filePath);

    fileData = fileReader.readFile(*filePath, ui->taskProgressBar);

    if (fileData == nullptr){
        QMessageBox::warning(
                    this,
                    "Error occured while reading .obj file",
                    "Incorrect path to file or incorrect data, please try again"
                    );
        return;
    } else {
        setLabelText(ui->taskLabel, "File successfully read");
    }

    glWidget->setFileData(fileData);

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

    setLabelText(ui->taskLabel, "Processing Object");

    if (!glWidget->generateArrays(ui->taskProgressBar)){
        taskMessageBox->addButton(QMessageBox::Ok);
        taskMessageBox->setIcon(QMessageBox::Information);
        taskMessageBox->setText("Ошибка");
        taskMessageBox->setInformativeText("Не получилось сгенерировать данные для отображения объекта");
        taskMessageBox->exec();

        setLabelText(ui->taskLabel, "Processing failed");

        return;
    }

    setLabelText(ui->taskLabel, "Object successfully processed");

    taskMessageBox->addButton(QMessageBox::Ok);
    taskMessageBox->setIcon(QMessageBox::Information);
    taskMessageBox->setText("Успех");
    taskMessageBox->setInformativeText("Массивы данных успешно сгенерированы");
    taskMessageBox->exec();

    delete taskMessageBox;
}

void MainWindow::addObject()
{

}

void MainWindow::setLabelText(QLabel *label, QString text)
{
    label->setText(text);
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

    // Add new object to a window action
    addObjectAction = new QAction(tr("Add object"), this);
    addObjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_A));
    addObjectAction->setStatusTip(tr("Add new object to a scene"));
    connect(addObjectAction, &QAction::triggered, this, &MainWindow::addObject);

}

void MainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    objectMenu = menuBar()->addMenu(tr("&Object"));
    objectMenu->addAction(showObjectAction);
    objectMenu->addAction(addObjectAction);

}

