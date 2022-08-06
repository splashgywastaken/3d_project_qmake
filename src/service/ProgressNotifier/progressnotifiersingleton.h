#ifndef PROGRESSNOTIFIERSINGLETON_H
#define PROGRESSNOTIFIERSINGLETON_H

#include "abstractprogressnotifier.h"

#include <qprogressbar.h>

class ProgressNotifierSingleton : public AbstractProgressNotifier
{
public:
    static ProgressNotifierSingleton *getInstance();
    static void initialize(QProgressBar *progressBar);

    void start(int minValue, int maxValue) override;
    void setProgress(int value) override;
    void finish() override;

    ProgressNotifierSingleton(ProgressNotifierSingleton& progressNotifier) = delete;

protected:
    ProgressNotifierSingleton() {};
    void setProgressBar(QProgressBar *progressBar);
    bool isInitialized() const;
    bool isRunning() const;

protected:
    static ProgressNotifierSingleton *m_instance;
    QProgressBar *m_progressBar = nullptr;
    bool m_isRunning = false;
};

#endif // PROGRESSNOTIFIERSINGLETON_H
