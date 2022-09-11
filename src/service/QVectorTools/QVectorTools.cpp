#include "QVectorTools.h"

QVector<int> QVectorTools::range(const int end)
{
    Q_ASSERT(end >= 0);
    QVector<int> result;
    for (int index = 0; index < end; index++)
    {
        result << index;
    }

    return result;
}
