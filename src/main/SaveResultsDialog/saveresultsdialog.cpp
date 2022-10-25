#include "saveresultsdialog.h"
#include "ui_saveresultsdialog.h"

#include <QFileDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QString>

SaveResultsDialog::SaveResultsDialog(
        QString windowTitle,
        QString mainText,
        QVector<double> transformationResultVector,
        QWidget *parent
        ) :
    QDialog(parent),
    ui(new Ui::SaveResultsDialog)
{
    ui->setupUi(this);
    m_transformationResultVector = new QVector<double>(transformationResultVector);
    m_isSaveDataChecked = true;
    m_isDeleteTargetChecked = true;

    // UI setup:
    this->setFixedSize(QSize(320, 270));
    setWindowTitle(windowTitle);
    QGridLayout* mainLayout = new QGridLayout();
    {
        // mainLabel setup
        {
            QLabel *mainLabel = new QLabel();

            if (!transformationResultVector.isEmpty())
            {
                mainLabel->setText(
                            QString(tr("Estimated transformation:\nTranslation (%1, %2, %3)\nRotation (%4, %5, %6)"))
                            .arg(transformationResultVector[0]).arg(transformationResultVector[1]).arg(transformationResultVector[2])
                        .arg(transformationResultVector[3]).arg(transformationResultVector[4]).arg(transformationResultVector[5])
                        );
                if (transformationResultVector.size() == 7)
                {
                    mainLabel->setText(mainLabel->text() + QString("\nScaling %7")
                                       .arg(transformationResultVector[6]));
                }
                else if (transformationResultVector.size() == 9)
                {
                    mainLabel->setText(mainLabel->text() + QString("\nScaling (%7, %8, %9)")
                                       .arg(transformationResultVector[6]).arg(transformationResultVector[7]).arg(transformationResultVector[8]));
                }
            }
            m_resultText = new QString(mainLabel->text());
            mainLabel->setText(mainLabel->text() + "\n" + mainText);

            mainLayout->addWidget(
                        mainLabel,
                        0, 0, 4, 3,
                        Qt::AlignCenter
                        );
        }

        // File CheckBox setup
        {
            m_saveResultsCheckBox = new QCheckBox(tr("Save results"));
            m_saveResultsCheckBox->setChecked(m_isSaveDataChecked);
            mainLayout->addWidget(
                        m_saveResultsCheckBox,
                        4, 0, 1, 3
                        );
        }

        // File path setup
        {
            QLabel* filePathLabel = new QLabel(tr("Set path to file to save to"));
            m_filePathLineEdit = new QLineEdit(tr("File path"));
            m_setFilePathButton = new QPushButton(tr("Browse..."));

            mainLayout->addWidget(
                        filePathLabel,
                        5, 0, 1, 3,
                        Qt::AlignCenter
                        );
            mainLayout->addWidget(
                        m_filePathLineEdit,
                        6, 0, 1, 2
                        );
            mainLayout->addWidget(
                        m_setFilePathButton,
                        6, 2, 1, 1
                        );
        }

        // Delete target object
        {
            m_deleteTargetCheckBox = new QCheckBox(tr("Delete target object"));
            m_deleteTargetCheckBox->setChecked(m_isDeleteTargetChecked);
            mainLayout->addWidget(
                        m_deleteTargetCheckBox,
                        7, 0, 1, 3
                        );
        }

        // Accept button setup
        {
            m_acceptButton = new QPushButton(tr("Accept"));

            mainLayout->addWidget(
                        m_acceptButton,
                        8, 0, 1, 3
                        );
        }

    }
    this->setLayout(mainLayout);

    // Connections
    connect(
            m_saveResultsCheckBox, &QCheckBox::toggled,
            this, &SaveResultsDialog::saveResultsCheckBoxToggled
            );
    connect(
            m_deleteTargetCheckBox, &QCheckBox::toggled,
            this, &SaveResultsDialog::deleteTargetCheckBoxToggled
            );
    connect(
            m_setFilePathButton, &QPushButton::clicked,
            this, &SaveResultsDialog::setFilePathButtonPressed
            );
    connect(
            m_acceptButton, &QPushButton::clicked,
            this, &SaveResultsDialog::acceptButtonPressed
            );
}

SaveResultsDialog::~SaveResultsDialog()
{
    delete ui;
}

void SaveResultsDialog::acceptButtonPressed(bool checked)
{
    Q_UNUSED(checked);

    if (m_isSaveDataChecked){
        saveData();
    }

    if (m_isDeleteTargetChecked)
    {
        accept();
        return;
    }
    reject();
    return;
}

void SaveResultsDialog::setFilePathButtonPressed(bool value)
{
    Q_UNUSED(value);

    QFileDialog *fileDialog = new QFileDialog();
    m_filePath = new QString(fileDialog->getOpenFileName(
                        this,
                        tr("Choose file"),
                        QDir::homePath(),
                        tr("Text file (*.txt)"),
                        nullptr,
                        QFileDialog::DontUseNativeDialog
                    ));
    if (m_filePath->isEmpty())
    {
        delete fileDialog;
        return;
    }

    m_filePathLineEdit->setText(*m_filePath);
    delete fileDialog;
}

void SaveResultsDialog::saveResultsCheckBoxToggled(bool value)
{
    m_isSaveDataChecked = value;
    m_filePathLineEdit->setEnabled(value);
    m_setFilePathButton->setEnabled(value);
}

void SaveResultsDialog::deleteTargetCheckBoxToggled(bool value)
{
    m_isDeleteTargetChecked = value;
}

void SaveResultsDialog::saveData()
{
    QFile file(*m_filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        stream << *m_resultText;

        file.close();
        qDebug() << "Writing finished";
    }
}
