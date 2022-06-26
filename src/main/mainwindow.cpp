#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <src/service/file_readers/ObjFileReader/ObjFileReader.h>
#include <src/widgets/customglwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileData = nullptr;
    opengGLHandler = new OpenGLHandler();

    glWidget = new CustomGLWidget();
    ui->openGLWidget = glWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_searchButtonFilePath_clicked()
{
    QString *filePath = new QString(QFileDialog::getOpenFileName(this, tr("Выберите файл"),
                                                     "E:/projects SSD/Qt/3d_project_qmake/res/obj/cube.obj",
                                                     tr("Object (*.obj)")
                                                     ));
    ui->filePathText->setText(*filePath);

    delete filePath;
}


void MainWindow::on_readButtonFilePath_clicked()
{
    ObjFileReader fileReader = ObjFileReader::Instantiate();
    if (ui->filePathText->toPlainText() != "")
    {
        fileData = fileReader.readFile(ui->filePathText->toPlainText(), ui->readProgressProgressBar);
    }

    if (fileData == nullptr){
        QMessageBox::warning(
                    this,
                    "Ошибка при чтении файла .obj",
                    "Неправильный путь к файлу или неправильный формат файла"
                    );
        return;
    }

    QString* fileDataDescription = fileData->getDescription();

    ui->readResultText->setText(*fileDataDescription);
}


void MainWindow::on_viewIbjectButton_clicked()
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

    // TODO: create pipeline for object to view
    arraysMap = opengGLHandler->generateArrays(fileData);

    if (arraysMap == nullptr)
    {
        QMessageBox::warning(
                    this,
                    "Ошибка",
                    "Не получилось сгенерировать массивы данных об объекте"
                    );

        return;
    }

    QMessageBox* successMessageBox = new QMessageBox();

    successMessageBox->addButton(QMessageBox::Ok);
    successMessageBox->setIcon(QMessageBox::Information);
    successMessageBox->setText("Успех");
    successMessageBox->setInformativeText("Массивы данных успешно сгенерированы");
    successMessageBox->exec();

    delete successMessageBox;

    // Attach vertices from data to open gl widget to show it
}

