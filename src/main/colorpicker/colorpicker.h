#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QDialog>

namespace Ui {
class colorpicker;
}

class colorpicker : public QDialog
{
    Q_OBJECT

public:
    explicit colorpicker(QWidget *parent = nullptr);
    ~colorpicker();

private:
    Ui::colorpicker *ui;
};

#endif // COLORPICKER_H
