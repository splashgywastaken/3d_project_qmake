#ifndef GRID_H
#define GRID_H

#include <QOpenGLBuffer>
#include <QColor>
#include <sceneobject.h>

using namespace DrawableObjectTools;

class Grid : public SceneObject
{
public:
    Grid(double size, int nSegments, QColor objectColor = QColor(65, 27, 87));

    void draw(
            QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix,
            QOpenGLShaderProgram *shader = nullptr, ShaderProgrammType shaderType = ShaderProgrammType::Standard
            ) override;
    void setObjectColor(QColor objectColor) override;
    QColor &getObjectColor() override;

private:
    QOpenGLBuffer* m_vertexBuffer;
    double m_size = 3.0;
    int m_nSegments;
    QColor m_objectColor;
};

#endif // GRID_H
