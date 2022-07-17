#ifndef FINDNEARESTPOINTDIALOG_H
#define FINDNEARESTPOINTDIALOG_H

#include <QDialog>
#include <node.h>

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
    void useRadiusCheckBoxChanged(bool value);
    void findNearestButtonClicked(bool checked);

protected:
    QVector3D* m_nearestPoint = nullptr;

    float* m_nearestPointDistSquared;
    QVector<QVector3D> m_points;
    QVector3D m_pointToSearch;
    bool m_useRadius;
    float m_radius;

    Ui::FindNearestPointDialog *m_ui;
};

#endif // FINDNEARESTPOINTDIALOG_H
