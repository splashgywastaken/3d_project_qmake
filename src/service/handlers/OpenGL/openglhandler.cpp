#include "openglhandler.h"

OpenGLHandler::OpenGLHandler()
{

}

QMap<QString, QVector<GLfloat> *> *OpenGLHandler::generateArrays(IObjFileData *fileData)
{

    // Data from fileData variable
    QVector<QVector3D*>* fileDataVertices = new QVector<QVector3D*>(fileData->getVertices());
    QVector<QVector3D*>* fileDataNormals = new QVector<QVector3D*>(fileData->getNormals());
    QVector<QVector2D*>* fileDataTextureCoordinates = new QVector<QVector2D*>(fileData->getVertexTextureCoordinates());
    QVector<QVector<QString*>*>* fileDataFaces = new QVector<QVector<QString*>*>(fileData->getFaces());

    // Vectors to form data in
    // Change to GLfloat* data type
    QVector<GLfloat>* vertices = new QVector<GLfloat>();
    QVector<GLfloat>* normals = new QVector<GLfloat>();
    QVector<GLfloat>* textureCoordinates = new QVector<GLfloat>();

    // Faces look like this: 1/1/1 2/2/2 3/3/3 where first num is vertex, second one is normal, third one is texCoord
    /// Algorythm

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
        }
    }

    // Result variable
    QMap<QString, QVector<GLfloat>*>* resultMap = new QMap<QString, QVector<GLfloat>*>();
    // Forming data to pass as return
    resultMap->insert("vertices", vertices);
    resultMap->insert("normals", normals);
    resultMap->insert("textureCoordinates", textureCoordinates);

    delete fileDataFaces;
    delete fileDataTextureCoordinates;
    delete fileDataNormals;
    delete fileDataVertices;

    return resultMap;
}
