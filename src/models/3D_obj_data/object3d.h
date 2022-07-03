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
    Object3D(QString inputObjectName, ObjFileData *inputFileData);
    ~Object3D() = default;

    bool generateData();

    void setFileData(ObjFileData* objData);

    QMap<QString, QList<GLfloat>*>* getObjectArrays();
    const GLfloat* getConstData(QString key) const;

    int getFacesCount() const;
    int getVertexCount() const;
    int getNormalsCount() const;
    int getTextureCoordinatesCount() const;

private:
    QString *objectName;
    QMap<QString, QList<GLfloat>*> *objectArrays;
    ObjFileData* fileData;
    int* facesCount;
    int* verticesCount;
    int* textureCoordinatesCount;
    int* normalsCount;
};

#endif // OBJECT3D_H
