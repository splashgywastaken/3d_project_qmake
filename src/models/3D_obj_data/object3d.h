#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <GL/gl.h>
#include <QString>
#include <QMap>
#include <QProgressBar>
#include <src/models/dto/ObjFileData/ObjFileData.h>

#include <src/service/GlobalState.h>

class Object3D
{
public:
    Object3D();
    ~Object3D() = default;

    bool generateData(ObjFileData& fileData, AbstractProgressNotifier* progressNotifier);

    QMap<QString, QList<GLfloat>>& getObjectArrays();
    const GLfloat* getConstData(QString key);

    int getFacesCount();
    int getVertexCount();
    int getNormalsCount();
    int getTextureCoordinatesCount();

private:
    QMap<QString, QList<GLfloat>> objectArrays;
    int facesCount;
    int verticesCount;
    int textureCoordinatesCount;
    int normalsCount;
};

#endif // OBJECT3D_H
