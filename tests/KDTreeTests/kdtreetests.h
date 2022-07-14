#ifndef KDTREE_KDTREETESTS_H
#define KDTREE_KDTREETESTS_H

#include <QTest>

namespace KDTree {

class KDTreeTests : public QObject
{
    Q_OBJECT
public:
    explicit KDTreeTests(QObject *parent = nullptr);
private slots:
    void testBuildTree01();

    void testFindNearestPoint01();
    void testFindNearestPoint02();
    void testFindNearestPoint03();
    void testFindNearestPoint04();
    void testFindNearestPoint05();
};

}

#endif
