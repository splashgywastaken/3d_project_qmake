#ifndef PROGRESSNOTIFIERSINGLETON_H
#define PROGRESSNOTIFIERSINGLETON_H

#include <qprogressbar.h>

class ProgressNotifierSingleton
{
public:
    static ProgressNotifierSingleton *getInstance();
    static void initialize(QProgressBar *progressBar);

    void start(int minValue, int maxValue);
    void setProgress(int value);
    void finish();

    ProgressNotifierSingleton(ProgressNotifierSingleton& progressNotifier) = delete;

protected:
    ProgressNotifierSingleton();
protected:
    static ProgressNotifierSingleton *m_instance;
    QProgressBar *m_progressBar = nullptr;
};

#endif // PROGRESSNOTIFIERSINGLETON_H
