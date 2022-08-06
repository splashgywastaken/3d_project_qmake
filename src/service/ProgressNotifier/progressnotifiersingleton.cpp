#include "progressnotifiersingleton.h"

#include "qdebug.h"
#include "qprogressbar.h"

ProgressNotifierSingleton *ProgressNotifierSingleton::m_instance = nullptr;

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
    getInstance()->m_progressBar->setVisible(true);
}

void ProgressNotifierSingleton::start(int minValue, int maxValue)
{
    Q_ASSERT(isInitialized());
    Q_ASSERT(!isRunning());
    m_isRunning = true;
    m_progressBar->setRange(minValue, maxValue);
    m_progressBar->setValue(minValue);
    m_progressBar->show();
}

void ProgressNotifierSingleton::setProgress(int value)
{
    Q_ASSERT(isInitialized());
    Q_ASSERT(value >= m_progressBar->minimum());
    Q_ASSERT(value <= m_progressBar->maximum());
    Q_ASSERT(isRunning());
    m_progressBar->setValue(value);
}

void ProgressNotifierSingleton::finish()
{
    Q_ASSERT(isInitialized());
    Q_ASSERT(isRunning());
    m_isRunning = false;
    m_progressBar->hide();
}

void ProgressNotifierSingleton::setProgressBar(QProgressBar *progressBar)
{
    m_progressBar = progressBar;
}

bool ProgressNotifierSingleton::isInitialized() const
{
    return m_progressBar != nullptr;
}

bool ProgressNotifierSingleton::isRunning() const
{
    return m_isRunning;
}
