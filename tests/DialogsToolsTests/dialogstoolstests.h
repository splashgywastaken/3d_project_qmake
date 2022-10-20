#ifndef DIALOGSTOOLS_DIALOGSTOOLSTESTS_H
#define DIALOGSTOOLS_DIALOGSTOOLSTESTS_H

#include <QObject>
#include <QTest>

namespace DialogsTools {
namespace Tests
{
class DialogsToolsTests : public QObject
{
    Q_OBJECT
private slots:
    void stringtoDoubleVectorTest01();
    void stringtoDoubleVectorTest02();
    void stringtoDoubleVectorTest03();
    void stringtoDoubleVectorTest04();
    void stringtoDoubleVectorTest05();

    void parseDouble01();
    void parseDouble02();
    void parseDouble03();

    void parseInt01();
    void parseInt02();
    void parseInt03();
};

}
}

#endif
