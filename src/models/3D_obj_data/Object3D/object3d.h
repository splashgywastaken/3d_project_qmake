#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <GL/gl.h>
#include <QString>
#include <QMap>
#include <QProgressBar>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <src/models/dto/ObjFileData/ObjFileData.h>
#include <src/models/3D_obj_data/SceneObject/sceneobject.h>

#include <src/service/GlobalState.h>

class Object3D : public SceneObject
{
public:
    Object3D(
            QVector<QVector3D> vertices, QVector<int> polygonVertexIndices,
            QVector<int> polygonStart,
            QVector<QVector3D> normals, QVector<int> polygonNormalIndices
            );
    ~Object3D();

    void draw(
            QMatrix4x4 viewMatrix,
            QMatrix4x4 projectionMatrix,
            QOpenGLShaderProgram* shader,
            DrawableObjectTools::ShaderProgrammType shaderType
            ) override;
    void setObjectColor(QVector3D objectColor) override;
    QVector3D& getObjectColor() override;
private:
    QOpenGLBuffer* m_vertexBuffer = nullptr;
    QOpenGLBuffer* m_normalBuffer  = nullptr;

    QVector3D m_objectColor = {1.0, 0.0, 0.0};

    int m_nVertices = -1;
};

#endif // OBJECT3D_H
