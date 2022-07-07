#include "drawableobjecttools.h"

QOpenGLShaderProgram *DrawableObjectTools::createShaderProgram(QString vertexShaderFilePath, QString fragmentShaderFilePath)
{
    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram();
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFilePath))
    {
        delete shader;
        return nullptr;
    }
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFilePath))
    {
        delete shader;
        return nullptr;
    }
    if (!shader->link())
    {
        delete shader;
        return nullptr;
    }

    return shader;
}
