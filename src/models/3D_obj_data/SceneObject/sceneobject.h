#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <src/service/GlobalState.h>

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

class SceneObject
{
public:
    SceneObject() = default;
    virtual ~SceneObject() = default;
    virtual void draw(
            QMatrix4x4 viewMatrix,
            QMatrix4x4 projectionMatrix,
            QOpenGLShaderProgram* shader,
            DrawableObjectTools::ShaderProgrammType shaderType
            ) = 0;
    virtual void setObjectColor(QVector3D objectColor) = 0;
    virtual QVector3D& getObjectColor() = 0;
};

#endif // SCENEOBJECT_H
