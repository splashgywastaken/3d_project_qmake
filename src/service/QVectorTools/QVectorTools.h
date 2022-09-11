#ifndef QVECTORTOOLS_H
#define QVECTORTOOLS_H

#include <QVector>

namespace QVectorTools {

template<typename Item>
QVector<Item> subset(const QVector<int>& indices, const QVector<Item>& vector);

QVector<int> range(const int end);

}

template<typename Item>
inline QVector<Item> QVectorTools::subset(const QVector<int>& indices, const QVector<Item>& vector)
{
    const int nOriginalItems = vector.size();
    QVector<Item> result;

    for (const int index : indices)
    {
        Q_ASSERT(index >= 0 && index < nOriginalItems);
        result.push_back(vector[index]);
    }

    return result;
}

#endif
