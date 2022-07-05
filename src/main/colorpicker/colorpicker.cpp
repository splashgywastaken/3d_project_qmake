#include "colorpicker.h"
#include "ui_colorpicker.h"

colorpicker::colorpicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::colorpicker)
{
    ui->setupUi(this);
}

colorpicker::~colorpicker()
{
    delete ui;
}
