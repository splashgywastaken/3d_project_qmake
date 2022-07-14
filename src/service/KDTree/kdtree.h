#ifndef KDTREE_H
#define KDTREE_H

#include "nodeinner.h"
#include "nodeleaf.h"

namespace KDTree
{

Node *buildTreeWithIndices(const QList<int>& pointIndices, const QVector<QVector3D>& points, int nPointsInLeaf = 1);
Node* buildTree(const QVector<QVector3D>& points, int nPointsInLeaf = 1);

};

#endif
