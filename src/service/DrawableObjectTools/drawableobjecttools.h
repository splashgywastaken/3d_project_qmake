#ifndef DRAWABLEOBJECTTOOLS_H
#define DRAWABLEOBJECTTOOLS_H

#include "qopenglshaderprogram.h"
#include <QString>


namespace DrawableObjectTools
{

QOpenGLShaderProgram *createShaderProgram(QString vertexShaderFilePath, QString fragmentShaderFilePath);

}

#endif // DRAWABLEOBJECTTOOLS_H
