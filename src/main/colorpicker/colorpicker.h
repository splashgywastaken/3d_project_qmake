#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QDialog>
#include <QVector3D>

#include <src/main/mainwindow/mainwindow.h>

namespace Ui {
class ColorPicker;
}

class ColorPicker : public QDialog
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = nullptr, MainWindow* mainWindow = nullptr, QVector3D currentColor = QVector3D(0.0, 0.0, 0.0));
    ~ColorPicker();

private slots:
    void redColorChanged(int color);
    void greenColorChanged(int color);
    void blueColorChanged(int color);

    void accept();

private:
    void updateColorLabel();

    Ui::ColorPicker *m_ui;
    MainWindow* m_mainWindow;
    QVector3D m_color;
};

#endif // COLORPICKER_H
