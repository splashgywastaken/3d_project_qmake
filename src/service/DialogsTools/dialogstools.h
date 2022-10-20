#ifndef DIALOGSTOOLS_H
#define DIALOGSTOOLS_H

#include <QString>
#include <QVector>

namespace DialogsTools
{
    // Gets string as x1, x2 ... xn,
    // returns vector QVector<double>{x1, x2, ... xn}
    bool stringToDoubleVector(const QString& string, QVector<double>& vector);

    bool parseDouble(const QString& string, double& value);
    bool parseInt(const QString& string, int& value);
}

#endif // DIALOGSTOOLS_H
