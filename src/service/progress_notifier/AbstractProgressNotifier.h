#ifndef ABSTRACTPROGRESSNOTIFIER_H
#define ABSTRACTPROGRESSNOTIFIER_H

class AbstractProgressNotifier
{
public:
    AbstractProgressNotifier() = default;
    virtual void start(int minValue, int maxValue) = 0;
    virtual void setProgress(int value) = 0;
    virtual void finish() = 0;
};
#endif // ABSTRACTPROGRESSNOTIFIER_H
