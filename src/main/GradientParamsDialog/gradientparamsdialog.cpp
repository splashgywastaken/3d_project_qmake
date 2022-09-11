#include "gradientparamsdialog.h"
#include "qpushbutton.h"

namespace Ui {

GradientParamsDialog::GradientParamsDialog(
        QVector<double> &variables,
        double &stepLength,
        int &nMaxIterations,
        double &gradientNormThreshold
        ) :
    m_variables(variables),
    m_stepLength(stepLength),
    m_nMaxIterations(nMaxIterations),
    m_gradientNormThreshold(gradientNormThreshold)
{
    m_variables = {0, 0, 0, 0, 0, 0, 1};
    stepLength = 0.4;
    nMaxIterations = 1500;
    gradientNormThreshold = 9e-7;


}

void GradientParamsDialog::acceptButtonClicked(bool checked)
{
    Q_UNUSED(checked);


}



}
