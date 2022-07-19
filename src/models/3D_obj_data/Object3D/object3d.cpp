#include "object3d.h"

#include <QDir>
#include <QMap>

#include <src/models/dto/ObjFileData/ObjFileData.h>

Object3D::Object3D(
        QVector<QVector3D> vertices, QVector<int> polygonVertexIndices,
        QVector<int> polygonStart,
        QVector<QVector3D> normals, QVector<int> polygonNormalIndices
        )
{
    QVector<int> triangleVertexIndices = MeshTools::buildTriangleVertexIndices(polygonVertexIndices, polygonStart);
    QVector<float> triangleVertexCoords = MeshTools::packTriangleVertexCoords(vertices, triangleVertexIndices);
    m_nVertices = triangleVertexCoords.size() / 3;
    int dataSize = triangleVertexCoords.size() * static_cast<int>(sizeof(float));

    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!m_vertexBuffer->create())
    {
        qFatal("Failed to create vertex buffer");
    }
    if (!m_vertexBuffer->bind())
    {
        qFatal("Failed to bind vertex buffer");
    }

    m_vertexBuffer->allocate(triangleVertexCoords.constData(), dataSize);
    m_vertexBuffer->release();

    QVector<int> triangleNormalIndices = MeshTools::buildTriangleVertexIndices(polygonNormalIndices, polygonStart);
    QVector<float> triangleNormalCoords = MeshTools::packTriangleVertexCoords(normals, triangleNormalIndices);

    m_normalBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!m_normalBuffer->create())
    {
        qFatal("Failed to create normal buffer");
    }
    if (!m_normalBuffer->bind())
    {
        qFatal("Failed to bind normal buffer");
    }

    m_normalBuffer->allocate(triangleNormalCoords.constData(), dataSize);
    m_normalBuffer->release();
}

Object3D::~Object3D()
{
    delete m_vertexBuffer;
    delete m_normalBuffer;
}

int Object3D::nVertices()
{
    return m_nVertices;
}

void Object3D::draw(
        QMatrix4x4 viewMatrix,
        QMatrix4x4 projectionMatrix,
        QOpenGLShaderProgram* shader,
        DrawableObjectTools::ShaderProgrammType shaderType
        )
{
    QMatrix4x4 normalMatrix = viewMatrix.inverted().transposed();

    bool isBound = shader->bind();
    Q_ASSERT(isBound);
    shader->setUniformValue("modelViewMatrix", viewMatrix);
    shader->setUniformValue("normalMatrix", normalMatrix);
    shader->setUniformValue("projectionMatrix", projectionMatrix);
    shader->setUniformValue("u_objectColor", m_objectColor);

    isBound = m_vertexBuffer->bind();
    Q_ASSERT(isBound);
    shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    shader->enableAttributeArray("vertex");
    m_vertexBuffer->release();

    if (shaderType == DrawableObjectTools::ShaderProgrammType::Lightning
            || shaderType == DrawableObjectTools::ShaderProgrammType::LightningWithTextures
            || shaderType == DrawableObjectTools::ShaderProgrammType::NormalMap
            )
    {
        isBound = m_normalBuffer->bind();
        Q_ASSERT(isBound);
        shader->setAttributeBuffer("normal", GL_FLOAT, 0, 3, 0);
        shader->enableAttributeArray("normal");
        m_vertexBuffer->release();
    }

    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, m_nVertices);

    shader->disableAttributeArray("vertex");
    if (shaderType == DrawableObjectTools::ShaderProgrammType::Lightning
            || shaderType == DrawableObjectTools::ShaderProgrammType::LightningWithTextures
            || shaderType == DrawableObjectTools::ShaderProgrammType::NormalMap
            )
    {
        shader->disableAttributeArray("normal");
    }

    shader->release();
}

void Object3D::setObjectColor(QVector3D objectColor)
{
    m_objectColor = objectColor;
}

QVector3D& Object3D::getObjectColor()
{
    return m_objectColor;
}
