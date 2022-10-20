#include "gradientparamsdialog.h"
#include "ui_gradientparamsdialog.h"

#include <float.h>
#include <QPushButton>
#include <QFile>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QList>
#include <src/service/DialogsTools/dialogstools.h>

GradientParamsDialog::GradientParamsDialog(
        int nVariables,
        QWidget* parent
        ) :
    QDialog(parent),
    m_ui(new Ui::GradientParamsDialog)
{
    m_nVariables = nVariables;
    m_variables = QVector<double>();
    m_variables.resize(nVariables);
    m_stepLength = 0.4;
    m_gradientNormThreshold = 9e-7;
    m_nMaxIterations = 1500;

    // Style sheet setup
    QFile styleFile(":/main/GradientParamsDialog/GradientParamsDialog.qss");
    styleFile.open(QFile::ReadOnly);
    this->setStyleSheet(QString::fromLatin1( styleFile.readAll() ));
    styleFile.close();

    //UI setup
    m_ui->setupUi(this);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    {
        QLabel* mainLabel = new QLabel(tr("Введите параметры для проведения оптимизации с использованием градиента"));
        mainLayout->addWidget(mainLabel);
        // Variables vector
        {
            QHBoxLayout* variablesLayout = new QHBoxLayout();

            QVector<QString> variablesStringList;
            for (auto& el : m_variables){
                variablesStringList << QString::number(el);
            }

            m_variablesLineEdit = new QLineEdit();
            m_variablesLineEdit->setText(
                        variablesStringList.join(",")
                        );
            variablesLayout->addWidget(
                        new QLabel(tr("Переменные\n( вводить в формате x1,x2,...,xn )\nВведите ") + QString::number(m_nVariables) + tr(" переменных"))
                        );
            variablesLayout->addWidget(m_variablesLineEdit);

            mainLayout->addLayout(variablesLayout);
        }

        // Step length
        {
            QHBoxLayout* stepLengthLayout = new QHBoxLayout();

            m_stepLengthDoubleSpinBox = new QDoubleSpinBox();
            m_stepLengthDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
            m_stepLengthDoubleSpinBox->setSingleStep(0.05);
            m_stepLengthDoubleSpinBox->setValue(m_stepLength);
            stepLengthLayout->addWidget(
                        new QLabel(tr("Длина шага:"))
                        );
            stepLengthLayout->addWidget(
                        m_stepLengthDoubleSpinBox
                        );

            mainLayout->addLayout(stepLengthLayout);
        }

        // Gradient norm threshold
        {
            QHBoxLayout* gradientNormThresholdLayout = new QHBoxLayout();

            m_gradientNormThresholdDoubleSpinBox = new QDoubleSpinBox();
            m_gradientNormThresholdDoubleSpinBox->setRange(-DBL_MAX, DBL_MAX);
            m_gradientNormThresholdDoubleSpinBox->setSingleStep(1e-7);
            m_gradientNormThresholdDoubleSpinBox->setDecimals(8);
            m_gradientNormThresholdDoubleSpinBox->setValue(m_gradientNormThreshold);
            gradientNormThresholdLayout->addWidget(
                        new QLabel(tr("Ограничение нормы градиента:"))
                        );
            gradientNormThresholdLayout->addWidget(
                        m_gradientNormThresholdDoubleSpinBox
                        );

            mainLayout->addLayout(gradientNormThresholdLayout);
        }

        // Max iterations
        {
            QHBoxLayout* nMaxIterationsLayout = new QHBoxLayout();

            m_nMaxIterationsSpinBox = new QSpinBox();
            m_nMaxIterationsSpinBox->setRange(-INT_MAX, INT_MAX);
            m_nMaxIterationsSpinBox->setSingleStep(10);
            m_nMaxIterationsSpinBox->setValue(m_nMaxIterations);
            nMaxIterationsLayout->addWidget(
                        new QLabel(tr("Максимальное количество итераций:"))
                        );
            nMaxIterationsLayout->addWidget(
                        m_nMaxIterationsSpinBox
                        );

            mainLayout->addLayout(nMaxIterationsLayout);
        }

        // Accept button
        {
            m_acceptButton = new QPushButton(tr("Подтвердить"));
            mainLayout->addWidget(m_acceptButton);
        }
    }
    setLayout(mainLayout);

    connect(
        m_acceptButton, &QPushButton::clicked,
        this, &GradientParamsDialog::acceptButtonClicked
        );
}

GradientParamsDialog::~GradientParamsDialog(){
    delete m_ui;
}

void GradientParamsDialog::acceptButtonClicked(bool checked)
{
    Q_UNUSED(checked);

    QString errorMessage;
    if (!setParametersFromDialogData(errorMessage)){
        QMessageBox::warning(
                            this,
                            tr("Ошибка данных"),
                            tr("Текст ошибки:") + errorMessage
                            );
        return;
    }

    emit parametersAreSet(
                m_variables,
                m_stepLength,
                m_nMaxIterations,
                m_gradientNormThreshold
                );
    accept();
}

bool GradientParamsDialog::setParametersFromDialogData(QString& errorMessage)
{
    using namespace DialogsTools;
    if (!parseVariablesLineEdit(m_variables, errorMessage)){
        return false;
    }

    // Setting parameters
    if (!parseDouble(m_stepLengthDoubleSpinBox->text().replace(",","."), m_stepLength)){
        errorMessage += tr("\nдлина шага введена неправильно");
        return false;
    }

    if (!parseDouble(m_gradientNormThresholdDoubleSpinBox->text().replace(",","."), m_gradientNormThreshold)){
        errorMessage += tr("\nограничение нормы градиента введено неправильно");
        return false;
    }

    if (!parseInt(m_nMaxIterationsSpinBox->text(), m_nMaxIterations)){
        errorMessage += tr("\nколичество итераций введено неправильно");
        return false;
    }

    // Data checks
    if (!(m_stepLength >= 1 || m_nMaxIterations >= 1)){
        errorMessage += tr("\nнеправильный диапазон данных");
        return false;
    }

    return true;
}

bool GradientParamsDialog::parseVariablesLineEdit(QVector<double>& result, QString& errorMessage)
{
    QString data = m_variablesLineEdit->text();
    QVector<double> variables;
    if (!DialogsTools::stringToDoubleVector(data, variables)){
        errorMessage += tr("\nОшибка при конвертации данных");
        return false;
    }

    if (variables.count() != m_nVariables){
        errorMessage += tr("\nОшибка при работе с переменными,\nнеправильное количество переменных");
        return false;
    }

    result = variables;
    return true;
}
