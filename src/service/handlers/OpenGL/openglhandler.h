#ifndef OPENGLHANDLER_H
#define OPENGLHANDLER_H

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QScreen>
#include <QtMath>

#include <src/models/dto/IObjFileData.h>
class OpenGLHandler
{
public:
    OpenGLHandler();

    // Generates vertices, normals and texture coordinates arrays according to faces element to draw objects in OpenGL
    QMap<QString, QVector<GLfloat>*>* generateArrays(IObjFileData* fileData);

private:

};

#endif // OPENGLHANDLER_H
