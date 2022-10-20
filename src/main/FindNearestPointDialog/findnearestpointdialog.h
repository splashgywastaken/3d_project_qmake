#ifndef FINDNEARESTPOINTDIALOG_H
#define FINDNEARESTPOINTDIALOG_H

#include "qtextedit.h"
#include <QDialog>
#include <QDoubleSpinBox>
#include <node.h>
#include <qcheckbox.h>

namespace Ui {
class FindNearestPointDialog;
}

class FindNearestPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindNearestPointDialog(
            QVector<QVector3D> points,
            KDTree::Node* kDTreeHead, QWidget *parent = nullptr
            );
    ~FindNearestPointDialog();

signals:
    void foundNearest(QVector3D nearestPoint);

protected:
    // Tree head
    KDTree::Node* m_kDTreeHead;

    void findNearestPoint();


protected slots:
    // Getting data from DoubleSpinBox widgets
    void xValueDoubleSpinBoxValueChanged(double value);
    void yValueDoubleSpinBoxValueChanged(double value);
    void zValueDoubleSpinBoxValueChanged(double value);
    void radiusValueDoubleSpinBoxValueChanged(double value);

    // CheckBox
    void useRadiusCheckBoxChanged(bool value);
    // Buttons
    void getResultButtonClicked(bool checked);

protected:
    QVector3D* m_nearestPoint = nullptr;

    float* m_nearestPointDistSquared;
    QVector<QVector3D> m_points;
    QVector3D m_pointToSearch;
    bool m_useRadius;
    float m_radius;

    // Ui
    Ui::FindNearestPointDialog *m_ui;

    // Accept button
    QPushButton* m_getResultButton;

    // Point input
    QDoubleSpinBox* m_xValueDoubleSpinBox;
    QDoubleSpinBox* m_yValueDoubleSpinBox;
    QDoubleSpinBox* m_zValueDoubleSpinBox;

    // Radius things
    // Radiobutton (search in radius)
    QCheckBox* m_useRadiusCheckBox;
    // Radius input
    QDoubleSpinBox* m_radiusValueDoubleSpinBox;

    // Result
    QLineEdit* m_nearestPointValueTextEdit;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // FINDNEARESTPOINTDIALOG_H
