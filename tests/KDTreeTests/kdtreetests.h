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

    // Find nearest by distance
    void testFindNearestPoint01();
    void testFindNearestPoint02();
    void testFindNearestPoint03();
    void testFindNearestPoint04();
    void testFindNearestPoint05();

    // Find nearest by radius
    void testFindNearestPointInRadius06();
    void testFindNearestPointInRadius07();
    void testFindNearestPointInRadius08();
    void testFindNearestPointInRadius09();
    void testFindNearestPointInRadius10();
};

}

#endif
