#ifndef GRID_H
#define GRID_H

#include <QOpenGLBuffer>
#include <QColor>
#include <sceneobject.h>

using namespace DrawableObjectTools;

class Grid : public SceneObject
{
public:
    Grid(double size, int nSegments, QVector3D objectColor = QVector3D(0.9, 0.9, 0.9));

    void draw(
            QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix,
            QOpenGLShaderProgram *shader = nullptr, ShaderProgrammType shaderType = ShaderProgrammType::Standard
            ) override;
    void setObjectColor(QVector3D objectColor) override;
    QVector3D &getObjectColor() override;

private:
    QOpenGLBuffer* m_vertexBuffer;
    double m_size = 3.0;
    int m_nSegments;
    QVector3D m_objectColor;
};

#endif // GRID_H
