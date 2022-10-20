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
    setWindowTitle(tr("Nearest point finder"));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    // Setting up main layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    {
        mainLayout->addWidget(
                    new QLabel(tr("Введите данные для поиска ближайшей точки"))
                    );
        // Nearest point data layout setup
        {
            QHBoxLayout* nearestPointDataLayout = new QHBoxLayout();
            mainLayout->addLayout(nearestPointDataLayout);
            nearestPointDataLayout->addSpacerItem(
                        new QSpacerItem(
                            10, 10,
                            QSizePolicy::Maximum, QSizePolicy::Minimum
                            )
                        );
            // X Data
            {
               QHBoxLayout* xLayout = new QHBoxLayout();

               xLayout->addWidget(
                           new QLabel("X")
                           );
               m_xValueDoubleSpinBox = new QDoubleSpinBox(0);
               m_xValueDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
               xLayout->addWidget(m_xValueDoubleSpinBox);

               nearestPointDataLayout->addLayout(xLayout);
            }
            // Y Data
            {
                QHBoxLayout* yLayout = new QHBoxLayout();

                yLayout->addWidget(
                            new QLabel("Y")
                            );
                m_yValueDoubleSpinBox = new QDoubleSpinBox(0);
                m_yValueDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
                yLayout->addWidget(m_yValueDoubleSpinBox);

                nearestPointDataLayout->addLayout(yLayout);
            }
            // Z Data
            {
                QHBoxLayout* zLayout = new QHBoxLayout();
                zLayout->addWidget(
                            new QLabel("Z")
                            );
                m_zValueDoubleSpinBox = new QDoubleSpinBox(0);
                m_zValueDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
                zLayout->addWidget(m_zValueDoubleSpinBox);

                nearestPointDataLayout->addLayout(zLayout);
            }
            nearestPointDataLayout->addSpacerItem(
                        new QSpacerItem(
                            10, 10,
                            QSizePolicy::Maximum, QSizePolicy::Minimum
                            )
                        );
        }

        // Radius data
        {
            // CheckBox
            m_useRadiusCheckBox = new QCheckBox(tr("Вести поиск в радиусе"));
            m_useRadiusCheckBox->setCheckState(Qt::Checked);
            mainLayout->addWidget(m_useRadiusCheckBox);
            // Radius data
            {
                QHBoxLayout* radiusDataLayout = new QHBoxLayout();
                radiusDataLayout->addWidget(
                            new QLabel(tr("Радиус"))
                            );
                m_radiusValueDoubleSpinBox = new QDoubleSpinBox(0);
                radiusDataLayout->addWidget(m_radiusValueDoubleSpinBox);

                mainLayout->addLayout(radiusDataLayout);
            }
        }

        m_getResultButton = new QPushButton(tr("Найти ближайшую точку"));
        mainLayout->addWidget(m_getResultButton);

        mainLayout->addWidget(
                    new QLabel(tr("Значение найденной точки"))
                    );
        m_nearestPointValueTextEdit = new QLineEdit();
        mainLayout->addWidget(m_nearestPointValueTextEdit);
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
        m_nearestPointValueTextEdit->setText("Ошибка при поиске ближайшей точки");
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
