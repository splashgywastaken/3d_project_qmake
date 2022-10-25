#include "findnearestpointdialog.h"
#include "qpushbutton.h"
#include "ui_findnearestpointdialog.h"

#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <float.h>

FindNearestPointDialog::FindNearestPointDialog(
        QVector<QVector3D> points,
        KDTree::Node* kDTreeHead, QWidget *parent
        ) :
    QDialog(parent),
    m_ui(new Ui::FindNearestPointDialog)
{
    m_kDTreeHead = kDTreeHead;
    m_pointToSearch = QVector3D(0, 0, 0);
    m_useRadius = true;
    m_radius = 0;
    m_points = points;
    m_nearestPointDistSquared = new float();

    // Style sheet setup
    QFile styleFile(":/main/FindNearestPointDialog/FindNearestPointDialog.qss");
    styleFile.open(QFile::ReadOnly);
    this->setStyleSheet(QString::fromLatin1( styleFile.readAll() ));
    styleFile.close();

    // UI creation
    m_ui->setupUi(this);
    this->setFixedSize(QSize(320, 204));
    setWindowTitle(tr("Nearest point finder parameters"));
    // Setting up main layout
    QGridLayout* mainLayout = new QGridLayout;
    {
        mainLayout->addWidget(
                    new QLabel(tr("Input data for your point")),
                    0, 0, 1, 5
                    );
        // Nearest point data layout setup
        {
            // X Data
            {
               mainLayout->addWidget(
                           new QLabel("X"),
                           1, 0, 1, 1, Qt::AlignRight
                           );
               m_xValueDoubleSpinBox = new QDoubleSpinBox(0);
               m_xValueDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
               mainLayout->addWidget(
                           m_xValueDoubleSpinBox,
                           1, 1, 1, 1, Qt::AlignLeft
                           );
            }
            // Y Data
            {
                mainLayout->addWidget(
                            new QLabel("Y"),
                            1, 2, 1, 1, Qt::AlignLeft
                            );
                m_yValueDoubleSpinBox = new QDoubleSpinBox(0);
                m_yValueDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
                mainLayout->addWidget(
                            m_yValueDoubleSpinBox,
                            1, 3, 1, 1, Qt::AlignLeft
                            );
            }
            // Z Data
            {
                mainLayout->addWidget(
                            new QLabel("Z"),
                            1, 4, 1, 1, Qt::AlignLeft
                            );
                m_zValueDoubleSpinBox = new QDoubleSpinBox(0);
                m_zValueDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
                mainLayout->addWidget(
                            m_zValueDoubleSpinBox,
                            1, 5, 1, 1, Qt::AlignLeft
                            );
            }
        }

        // Radius data
        {
            // CheckBox
            m_useRadiusCheckBox = new QCheckBox(tr("Search in radius"));
            m_useRadiusCheckBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
            m_useRadiusCheckBox->setCheckState(Qt::Checked);
            mainLayout->addWidget(
                        m_useRadiusCheckBox,
                        2, 0, 1, 5, Qt::AlignLeft
                        );
            // Radius data
            {
                mainLayout->addWidget(
                            new QLabel(tr("Radius value")),
                            3, 0, 1, 3
                            );
                m_radiusValueDoubleSpinBox = new QDoubleSpinBox(0);
                mainLayout->addWidget(
                            m_radiusValueDoubleSpinBox,
                            3, 4, 1, 3
                            );
            }
        }

        m_getResultButton = new QPushButton(tr("Find closest point"));
        mainLayout->addWidget(
                    m_getResultButton,
                    4, 0, 1, 6
                    );
        mainLayout->addWidget(
                    new QLabel(tr("Closest point value")),
                    5, 0, 1, 5, Qt::AlignLeft
                    );
        m_nearestPointValueTextEdit = new QLineEdit(tr("search result"));
        m_nearestPointValueTextEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        m_nearestPointValueTextEdit->setReadOnly(true);
        mainLayout->addWidget(
                    m_nearestPointValueTextEdit,
                    6, 0, 1, 6
                    );
    }
    setLayout(mainLayout);

    // Connections setup
    connect(
        m_xValueDoubleSpinBox, &QDoubleSpinBox::valueChanged,
        this, &FindNearestPointDialog::xValueDoubleSpinBoxValueChanged
        );
    connect(
        m_yValueDoubleSpinBox, &QDoubleSpinBox::valueChanged,
        this, &FindNearestPointDialog::yValueDoubleSpinBoxValueChanged
        );
    connect(
        m_zValueDoubleSpinBox, &QDoubleSpinBox::valueChanged,
        this, &FindNearestPointDialog::zValueDoubleSpinBoxValueChanged
        );
    connect(
        m_radiusValueDoubleSpinBox, &QDoubleSpinBox::valueChanged,
        this, &FindNearestPointDialog::radiusValueDoubleSpinBoxValueChanged
        );
    connect(
        m_useRadiusCheckBox, &QCheckBox::toggled,
        this, &FindNearestPointDialog::useRadiusCheckBoxChanged
        );
    connect(
        m_getResultButton, &QPushButton::clicked,
        this, &FindNearestPointDialog::getResultButtonClicked
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

void FindNearestPointDialog::xValueDoubleSpinBoxValueChanged(double value)
{
    m_pointToSearch.setX(
                value
                );
}

void FindNearestPointDialog::yValueDoubleSpinBoxValueChanged(double value)
{
    m_pointToSearch.setY(
                value
                );
}

void FindNearestPointDialog::zValueDoubleSpinBoxValueChanged(double value)
{
    m_pointToSearch.setZ(
                value
                );
}

void FindNearestPointDialog::radiusValueDoubleSpinBoxValueChanged(double value)
{
     m_radius = value;
}

void FindNearestPointDialog::useRadiusCheckBoxChanged(bool value)
{
    m_useRadius = value;
    m_radiusValueDoubleSpinBox->setEnabled(value);
}

void FindNearestPointDialog::getResultButtonClicked(bool checked)
{
    Q_UNUSED(checked);
    findNearestPoint();

    if (m_nearestPoint == nullptr)
    {
        m_nearestPointValueTextEdit->setText("Error while searching for closest point");
    }
    else
    {
        m_nearestPointValueTextEdit->setText(
                    QString("(%1, %2, %3)")
                    .arg(m_nearestPoint->x())
                    .arg(m_nearestPoint->y())
                    .arg(m_nearestPoint->z())
                    );
    }
}

void FindNearestPointDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if (m_nearestPoint != nullptr)
    {
        emit foundNearest(*m_nearestPoint);
    }
}
