#include "colorpicker.h"
#include "ui_colorpicker.h"

#include <src/main/mainwindow/mainwindow.h>

ColorPicker::ColorPicker(QWidget *parent, MainWindow* mainWindow, QVector3D currentColor) :
    QDialog(parent),
    ui(new Ui::ColorPicker)
{
    ui->setupUi(this);

    // Setting up values:
    m_mainWindow = mainWindow;
    m_color = currentColor;

    // Styles
    ui->redLabel->setStyleSheet("color: red");
    ui->greenLabel->setStyleSheet("color: green");
    ui->blueLabel->setStyleSheet("color: blue");

    // Sliders
    connect(ui->redSlider, &QSlider::valueChanged, this, &ColorPicker::redColorChanged);
    connect(ui->greenSlider, &QSlider::valueChanged, this, &ColorPicker::greenColorChanged);
    connect(ui->blueSlider, &QSlider::valueChanged, this, &ColorPicker::blueColorChanged);

    // Button
    connect(ui->acceptButton, &QPushButton::clicked, this, &ColorPicker::accept);

    // Update
    ui->redSlider->setValue(currentColor.x() * 255);
    ui->greenSlider->setValue(currentColor.y() * 255);
    ui->blueSlider->setValue(currentColor.z() * 255);
    updateColorLabel();
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

void ColorPicker::redColorChanged(int color)
{
    m_color.setX(color);
    updateColorLabel();
}

void ColorPicker::greenColorChanged(int color)
{
    m_color.setY(color);
    updateColorLabel();
}

void ColorPicker::blueColorChanged(int color)
{
    m_color.setZ(color);
    updateColorLabel();
}

void ColorPicker::accept()
{
    m_mainWindow->setObjectColor(m_color);
    this->close();
}

void ColorPicker::updateColorLabel()
{
    ui->colorLabel->setStyleSheet(
                "background-color: rgb(" +
                QString::number(m_color.x()) + ", " +
                QString::number(m_color.y()) + ", " +
                QString::number(m_color.z()) +
                ")");
    ui->redValueLabel->setText(QString::number(m_color.x()));
    ui->greenValueLabel->setText(QString::number(m_color.y()));
    ui->blueValueLabel->setText(QString::number(m_color.z()));
}
