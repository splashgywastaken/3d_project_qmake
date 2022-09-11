#ifndef OPTIMIZATION_FITTINGPARAMSDIALOG_H
#define OPTIMIZATION_FITTINGPARAMSDIALOG_H

#include <QDialog>
#include <QObject>

namespace Ui {

class GradientParamsDialog : public QDialog
{
    Q_OBJECT
public:
    GradientParamsDialog(
            QVector<double> &variables,
            double &stepLength,
            int &nMaxIterations,
            double &gradientNormThreshold
            );

signals:
    void parametersSet(
            QVector<double> &variables,
            double &stepLength,
            int &nMaxIterations,
            double &gradientNormThreshold
            );

protected slots:
    void acceptButtonClicked(bool checked);

private:
    QVector<double>& m_variables;
    double& m_stepLength;
    int& m_nMaxIterations;
    double& m_gradientNormThreshold;

    Ui::GradientParamsDialog* m_ui;
};

}

#endif
