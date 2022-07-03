#include "progressnotifiersingleton.h"

#include "qprogressbar.h"

ProgressNotifierSingleton *ProgressNotifierSingleton::m_instance = nullptr;

ProgressNotifierSingleton::ProgressNotifierSingleton()
{
}

ProgressNotifierSingleton *ProgressNotifierSingleton::getInstance()
{
    if (m_instance == nullptr){
        m_instance = new ProgressNotifierSingleton;
    }
    return ProgressNotifierSingleton::m_instance;
}

void ProgressNotifierSingleton::initialize(QProgressBar *progressBar)
{
    if (m_instance != nullptr)
    {
        m_instance = new ProgressNotifierSingleton;
    }

    getInstance()->m_progressBar = progressBar;
}

void ProgressNotifierSingleton::start(int minValue, int maxValue)
{
    m_progressBar->setRange(minValue, maxValue);
}

void ProgressNotifierSingleton::setProgress(int value)
{
    m_progressBar->setValue(value);
}

void ProgressNotifierSingleton::finish()
{
    if (getInstance()->m_progressBar == nullptr)
    {
        return;
    }

    setProgress(getInstance()->m_progressBar->maximum());
}
