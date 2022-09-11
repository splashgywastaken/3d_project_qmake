#ifndef KDTREE_TIMETESTER_H
#define KDTREE_TIMETESTER_H

#include "kdtree.h"
#include "ObjFileReader.h"
#include "ObjFileData.h"

#include <QFileDialog>
#include <QElapsedTimer>
#include <QDebug>

namespace KDTree
{
namespace TimeTester
{
// Triangulates obj from fileName file, builds tree and runs algorythm to find nearest point index
void testBuildTreeFromFile01(QString fileName, QVector3D pointToSearch = {0.01, 0.01, 0.01});
};
}

#endif
