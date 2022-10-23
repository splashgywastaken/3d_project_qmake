#ifndef OPTIMIZATION_FITTINGPARAMSDIALOG_H
#define OPTIMIZATION_FITTINGPARAMSDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QObject>

namespace Ui {
class GradientParamsDialog;
}

class GradientParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GradientParamsDialog(
            int nVariables,
            QWidget* parent = nullptr
            );
    ~GradientParamsDialog();



signals:
    void parametersAreSet(
            QVector<double> variables,
            double stepLength,
            int nMaxIterations,
            double gradientNormThreshold
            );

protected slots:
    void acceptButtonClicked(bool checked);

private:
    bool setParametersFromDialogData(QString& errorMessage);
    bool parseVariablesLineEdit(QVector<double>& result, QString& errorMessage);

private:
    int m_nVariables;
    QVector<double> m_variables;
    double m_stepLength;
    int m_nMaxIterations;
    double m_gradientNormThreshold;

    // UI
    Ui::GradientParamsDialog* m_ui;

    QPushButton* m_acceptButton;
    QDoubleSpinBox* m_stepLengthDoubleSpinBox;
    QDoubleSpinBox* m_gradientNormThresholdDoubleSpinBox;
    QSpinBox* m_nMaxIterationsSpinBox;
    QLineEdit* m_variablesLineEdit;

    //
    // Overrides
    //

    // QWidget interface
public:
    QSize sizeHint() const override;
};
#endif
