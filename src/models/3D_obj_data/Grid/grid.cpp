#include "grid.h"

Grid::Grid(double size, int nSegments, QVector3D objectColor) :
    m_size(size),
    m_nSegments(nSegments),
    m_objectColor(objectColor)
{
}

void Grid::draw(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader, ShaderProgrammType shaderType)
{
    Q_UNUSED(shader);
    Q_UNUSED(shaderType);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(viewMatrix.data());
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.data());
    glColor3f(m_objectColor.x(), m_objectColor.y(), m_objectColor.z());
    glBegin(GL_LINES);
        const double segmentSize = m_size / m_nSegments;
        for (int segmentInd = 0; segmentInd < m_nSegments * 2 + 1; ++ segmentInd) {
            glVertex3f(-m_size + segmentInd * segmentSize, 0, -m_size);
            glVertex3f(-m_size + segmentInd * segmentSize, 0, m_size);
            glVertex3f(-m_size, 0, -m_size + segmentInd * segmentSize);
            glVertex3f(m_size, 0, -m_size + segmentInd * segmentSize);
        }
    glEnd();
}

void Grid::setObjectColor(QVector3D objectColor)
{
    m_objectColor = objectColor;
}

QVector3D &Grid::getObjectColor()
{
    return m_objectColor;
}
