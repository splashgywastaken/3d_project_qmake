#ifndef NODEINNER_H
#define NODEINNER_H

#include <node.h>


namespace KDTree {

enum Child {
    Child_Left = 0,
    Child_Right = 1
};

class NodeInner : public Node
{
public:
    explicit NodeInner(const Node* childLeft, const Node* childRight, int splitAxis, float splitPos);
    explicit NodeInner(const NodeInner& other);
    ~NodeInner();

    void findNearestPoint(const QVector3D &point, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const override;
    int findNearestPointIndex(const QVector3D &point, const QVector<QVector3D> &points, float *nearestPointDistSquared = nullptr) const override;
    void findNearestPoint(int pointInd, const QVector<QVector3D> &points, int &nearestPointInd, float &nearestPointDistSquared) const override;
    int findNearestPointIndex(int pointInd, const QVector<QVector3D> &points, float *nearestPointDistSquared = nullptr) const override;

    int memUsage() const override;
    Node* copy() const override;

public:
    const Node* m_childLeft;
    const Node* m_childRight;
    int m_splitAxis;
    float m_splitPos;
    const Node* child(Child child) const;
};

}

#endif // NODEINNER_H
