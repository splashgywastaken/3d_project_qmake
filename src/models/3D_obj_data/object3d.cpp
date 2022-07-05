#include "object3d.h"

#include <QMap>

#include <src/models/dto/ObjFileData/ObjFileData.h>

Object3D::Object3D()
{
}

bool Object3D::generateData(ObjFileData& fileData, AbstractProgressNotifier* progressNotifier)
{
    // Return false and assert if fileData is empty
    if (fileData.isEmpty())
    {
        qFatal("Failed to generateData. fileData member was empty in Object");
        return false;
    }

    // Data from fileData variable
    if (fileData.getVertices().count() == 0)
    {
        qFatal("Vertices array is empty");
        return false;
    }
    QVector<QVector3D> fileDataVertices = fileData.getVertices();
    QVector<QVector3D> fileDataNormals = fileData.getNormals();
    QVector<QVector2D> fileDataTextureCoordinates = fileData.getVertexTextureCoordinates();
    QVector<QVector<QString>> fileDataFaces = fileData.getFaces();

    // Vectors to form data in
    // Change to GLfloat* data type
    QList<GLfloat> vertices;
    QList<GLfloat> normals;
    QList<GLfloat> textureCoordinates;

    if (progressNotifier != nullptr)
    {
        // Setting up progressBar
        progressNotifier->start(0, fileDataFaces.count() * 3);
        progressNotifier->setProgress(0);
    }

    int progressStep = 0;


    // Viewing each triples of faces in list of faces data
    for (QVector<QString> &faces : fileDataFaces)
    {
        // Viewing each face in triples of faces:
        for (QString &face : faces)
        {
            // Converts string that contains data about a face to work with as a QList of ints
            QStringList* faceData = new QList(face.split("/").toList());

            // Adding vertices
            vertices.append(fileDataVertices.at(faceData->at(0).toInt() - 1).x());
            vertices.append(fileDataVertices.at(faceData->at(0).toInt() - 1).y());
            vertices.append(fileDataVertices.at(faceData->at(0).toInt() - 1).z());

            // Adding textureCoordinates
            textureCoordinates.append(fileDataTextureCoordinates.at(faceData->at(1).toInt() - 1).x());
            textureCoordinates.append(fileDataTextureCoordinates.at(faceData->at(1).toInt() - 1).y());

            // Adding normals
            normals.append(fileDataNormals.at(faceData->at(2).toInt() - 1).x());
            normals.append(fileDataNormals.at(faceData->at(2).toInt() - 1).y());
            normals.append(fileDataNormals.at(faceData->at(2).toInt() - 1).z());

            delete faceData;

            if (progressNotifier != nullptr)
            {
                progressNotifier->setProgress(++progressStep);
            }

        }
    }

    if (progressNotifier != nullptr)
    {
        progressNotifier->finish();
    }

    // Return false if data wasn't generated
    if (vertices.count() == 0 && normals.count() == 0 && textureCoordinates.count() == 0){
        return false;
    }

    // Return true if data was generated and assign pointer to a field for data to be in
    // Result variable
    QMap<QString, QList<GLfloat>> resultMap;
    // Forming data to pass as return
    resultMap.insert("vertices", vertices);
    resultMap.insert("normals", normals);
    resultMap.insert("textureCoordinates", textureCoordinates);

    facesCount = vertices.count() / 3;
    verticesCount = vertices.count() / 3;
    normalsCount = normals.count() / 3;
    textureCoordinatesCount = textureCoordinates.count() / 2;

    // Assigning data to a field of current instance
    objectArrays = resultMap;

    return true;
}

QMap<QString, QList<GLfloat>>& Object3D::getObjectArrays()
{
    return objectArrays;
}

const GLfloat* Object3D::getConstData(QString key)
{
    return objectArrays.take(key).constData();
}

int Object3D::getFacesCount()
{
    return facesCount;
}

int Object3D::getVertexCount()
{
    return verticesCount;
}

int Object3D::getNormalsCount()
{
    return normalsCount;
}

int Object3D::getTextureCoordinatesCount()
{
    return textureCoordinatesCount;
}

