#include "findnearestpointdialog.h"
#include "ui_findnearestpointdialog.h"

FindNearestPointDialog::FindNearestPointDialog(
        QVector<QVector3D> points,
        KDTree::Node* kDTreeHead, QWidget *parent
        ) :
    QDialog(parent),
    m_ui(new Ui::FindNearestPointDialog)
{
    m_ui->setupUi(this);

    m_kDTreeHead = kDTreeHead;
    m_pointToSearch = QVector3D(0, 0, 0);
    m_useRadius = true;
    m_radius = 0;
    m_points = points;
    m_nearestPointDistSquared = new float();

    connect(
        m_ui->useRadiusCheckBox, &QCheckBox::toggled,
        this, &FindNearestPointDialog::useRadiusCheckBoxChanged
        );
    connect(
        m_ui->findNearestPointButton, &QPushButton::clicked,
        this, &FindNearestPointDialog::findNearestButtonClicked
        );
}

FindNearestPointDialog::~FindNearestPointDialog()
{
    delete m_ui;
}

void FindNearestPointDialog::findNearestPoint()
{
    int pointIndex;

    if (m_useRadius)
    {
        pointIndex = m_kDTreeHead->findNearestPointIndexInRadius(m_pointToSearch, m_points, m_radius, m_nearestPointDistSquared);
    }
    else
    {
        pointIndex = m_kDTreeHead->findNearestPointIndex(m_pointToSearch, m_points, m_nearestPointDistSquared);
    }

    if (pointIndex != -1)
    {
        m_nearestPoint = new QVector3D(m_points[pointIndex]);
    }
}

void FindNearestPointDialog::useRadiusCheckBoxChanged(bool value)
{
    m_useRadius = value;
}

void FindNearestPointDialog::findNearestButtonClicked(bool checked)
{
    Q_UNUSED(checked);
    m_pointToSearch = QVector3D(
                m_ui->xValue->toPlainText().toFloat(),
                m_ui->yValue->toPlainText().toFloat(),
                m_ui->zValue->toPlainText().toFloat()
                );
    m_radius = m_ui->radiusValue->toPlainText().toFloat();

    findNearestPoint();

    if (m_nearestPoint == nullptr)
    {
        m_ui->nearestPointValue->setPlainText("Ошибка при поиске ближайшей точки");
    }
    else
    {
        m_ui->nearestPointValue->setPlainText(
                    QString("(%1, %2, %3)")
                    .arg(m_nearestPoint->x())
                    .arg(m_nearestPoint->y())
                    .arg(m_nearestPoint->z())
                    );
        emit foundNearest(*m_nearestPoint);
    }
}
