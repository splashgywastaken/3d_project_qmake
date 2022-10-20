#include "dialogstools.h"

bool DialogsTools::stringToDoubleVector(const QString &string, QVector<double> &vector)
{

    QVector<QString> splittedData = string.split(',', Qt::SkipEmptyParts).toVector();
    if (splittedData.isEmpty()){
        return false;
    }

    QVector<double> result;
    bool* ok = new bool(false);

    for (auto& el : splittedData){
        result << el.toDouble(ok);
        if (!*ok){
            delete ok;
            return false;
        }
    }

    delete ok;
    vector = result;
    return true;
}

bool DialogsTools::parseDouble(const QString &string, double &value)
{
    if (string.isEmpty()){
        return false;
    }

    bool ok;
    value = string.toDouble(&ok);

    return ok;
}

bool DialogsTools::parseInt(const QString &string, int &value)
{
    if (string.isEmpty()){
        return false;
    }

    bool ok;
    value = string.toInt(&ok);

    return ok;
}
