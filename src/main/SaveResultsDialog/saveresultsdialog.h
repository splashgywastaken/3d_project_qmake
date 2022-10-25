#ifndef SAVERESULTSDIALOG_H
#define SAVERESULTSDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class SaveResultsDialog;
}

class SaveResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveResultsDialog(
            QString windowTitle,
            QString mainText,
            QVector<double> transformationResultVector,
            QWidget *parent = nullptr
            );
    ~SaveResultsDialog();

private slots:
    void acceptButtonPressed(bool checked);
    void setFilePathButtonPressed(bool value);
    void saveResultsCheckBoxToggled(bool value);
    void deleteTargetCheckBoxToggled(bool value);

private:
    void saveData();

private:
    // UI
    Ui::SaveResultsDialog *ui;

    QCheckBox* m_saveResultsCheckBox;
    QCheckBox* m_deleteTargetCheckBox;
    QLineEdit* m_filePathLineEdit;
    QPushButton* m_setFilePathButton;
    QPushButton* m_acceptButton;

    // Fields
    QVector<double>* m_transformationResultVector;
    QString* m_filePath;
    QString* m_resultText;
    bool m_isSaveDataChecked;
    bool m_isDeleteTargetChecked;
};

#endif // SAVERESULTSDIALOG_H
