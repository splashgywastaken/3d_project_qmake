#include "colorpicker.h"
#include "ui_colorpicker.h"

#include <src/main/mainwindow/mainwindow.h>

ColorPicker::ColorPicker(QWidget *parent, MainWindow* mainWindow, QVector3D currentColor) :
    QDialog(parent),
    m_ui(new Ui::ColorPicker)
{
    m_ui->setupUi(this);

    // Setting up values:
    m_mainWindow = mainWindow;
    m_color = currentColor;

    // Styles
    m_ui->redLabel->setStyleSheet("color: red");
    m_ui->greenLabel->setStyleSheet("color: green");
    m_ui->blueLabel->setStyleSheet("color: blue");

    // Sliders
    connect(m_ui->redSlider, &QSlider::valueChanged, this, &ColorPicker::redColorChanged);
    connect(m_ui->greenSlider, &QSlider::valueChanged, this, &ColorPicker::greenColorChanged);
    connect(m_ui->blueSlider, &QSlider::valueChanged, this, &ColorPicker::blueColorChanged);

    // Button
    connect(m_ui->acceptButton, &QPushButton::clicked, this, &ColorPicker::accept);

    // Update
    m_ui->redSlider->setValue(currentColor.x() * 255);
    m_ui->greenSlider->setValue(currentColor.y() * 255);
    m_ui->blueSlider->setValue(currentColor.z() * 255);
    updateColorLabel();
}

ColorPicker::~ColorPicker()
{
    delete m_ui;
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
    m_ui->colorLabel->setStyleSheet(
                "background-color: rgb(" +
                QString::number(m_color.x()) + ", " +
                QString::number(m_color.y()) + ", " +
                QString::number(m_color.z()) +
                ")");
    m_ui->redValueLabel->setText(QString::number(m_color.x()));
    m_ui->greenValueLabel->setText(QString::number(m_color.y()));
    m_ui->blueValueLabel->setText(QString::number(m_color.z()));
}
