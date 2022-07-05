#include "colorpicker.h"
#include "ui_colorpicker.h"

#include <src/main/mainwindow/mainwindow.h>

ColorPicker::ColorPicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorPicker)
{
    ui->setupUi(this);

    // Sliders
    connect(ui->redSlider, &QSlider::valueChanged, this, &ColorPicker::redColorChanged);
    connect(ui->greenSlider, &QSlider::valueChanged, this, &ColorPicker::greenColorChanged);
    connect(ui->blueSlider, &QSlider::valueChanged, this, &ColorPicker::blueColorChanged);

    // Button
    connect(ui->acceptButton, &QPushButton::clicked, this, &ColorPicker::accept);
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

void ColorPicker::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
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
}
