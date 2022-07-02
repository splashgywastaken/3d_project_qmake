#include "object3d.h"

#include <QMap>

#include <src/models/dto/ObjFileData/ObjFileData.h>

Object3D::Object3D(QString inputObjectName, ObjFileData *inputFileData)
{
    objectName = new QString(inputObjectName);
    objectArrays = nullptr;
    fileData = inputFileData;

    facesCount = new int();
    normalsCount = new int();
    verticesCount = new int();
    textureCoordinatesCount = new int();
}

bool Object3D::generateData(QProgressBar* progressBar)
{
    // Data from fileData variable
    QVector<QVector3D*>* fileDataVertices = new QVector<QVector3D*>(fileData->getVertices());
    QVector<QVector3D*>* fileDataNormals = new QVector<QVector3D*>(fileData->getNormals());
    QVector<QVector2D*>* fileDataTextureCoordinates = new QVector<QVector2D*>(fileData->getVertexTextureCoordinates());
    QVector<QVector<QString*>*>* fileDataFaces = new QVector<QVector<QString*>*>(fileData->getFaces());

    // Vectors to form data in
    // Change to GLfloat* data type
    QList<GLfloat>* vertices = new QList<GLfloat>();
    QList<GLfloat>* normals = new QList<GLfloat>();
    QList<GLfloat>* textureCoordinates = new QList<GLfloat>();

    // Faces look like this: 1/1/1 2/2/2 3/3/3 where first num is vertex, second one is normal, third one is texCoord

    progressBar->setRange(0, fileDataFaces->count() * 3);

    int* progressBarStep = new int(0);

    for (QVector<QString*>* faces : *fileDataFaces)
    {
        for (QString* face : *faces)
        {
            QStringList* faceData = new QList(face->split("/").toList());

            // Adding vertices
            vertices->append(fileDataVertices->at(faceData->at(0).toInt() - 1)->x());
            vertices->append(fileDataVertices->at(faceData->at(0).toInt() - 1)->y());
            vertices->append(fileDataVertices->at(faceData->at(0).toInt() - 1)->z());

            // Adding textureCoordinates
            textureCoordinates->append(fileDataTextureCoordinates->at(faceData->at(1).toInt() - 1)->x());
            textureCoordinates->append(fileDataTextureCoordinates->at(faceData->at(1).toInt() - 1)->y());

            // Adding normals
            normals->append(fileDataNormals->at(faceData->at(2).toInt() - 1)->x());
            normals->append(fileDataNormals->at(faceData->at(2).toInt() - 1)->y());
            normals->append(fileDataNormals->at(faceData->at(2).toInt() - 1)->z());

            delete faceData;

            progressBar->setValue(++(*progressBarStep));

        }
    }

    delete progressBarStep;

    // Result variable
    QMap<QString, QList<GLfloat>*>* resultMap = new QMap<QString, QList<GLfloat>*>();
    // Forming data to pass as return
    resultMap->insert("vertices", vertices);
    resultMap->insert("normals", normals);
    resultMap->insert("textureCoordinates", textureCoordinates);

    *facesCount = vertices->count() / 3;
    *verticesCount = vertices->count() / 3;
    *normalsCount = normals->count() / 3;
    *textureCoordinatesCount = textureCoordinates->count() / 2;

    delete fileDataFaces;
    delete fileDataTextureCoordinates;
    delete fileDataNormals;
    delete fileDataVertices;

    objectArrays = resultMap;

    return true;
}

void Object3D::setFileData(ObjFileData *objData)
{
    fileData = objData;
}

QMap<QString, QList<GLfloat> *> *Object3D::getObjectArrays()
{
    return objectArrays;
}

const GLfloat* Object3D::getConstData(QString key) const
{
    return objectArrays->take(key)->constData();
}

int Object3D::getFacesCount() const
{
    return *facesCount;
}

int Object3D::getVertexCount() const
{
    return *verticesCount;
}

int Object3D::getNormalsCount() const
{
    return *normalsCount;
}

int Object3D::getTextureCoordinatesCount() const
{
    return *textureCoordinatesCount;
}

