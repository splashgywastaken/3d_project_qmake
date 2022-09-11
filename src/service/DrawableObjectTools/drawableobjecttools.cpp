#include "drawableobjecttools.h"

DrawableObjectTools::ResourceManager* DrawableObjectTools::ResourceManager::m_instance = nullptr;

DrawableObjectTools::ResourceManager *DrawableObjectTools::ResourceManager::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ResourceManager();
    }

    return m_instance;
}

bool DrawableObjectTools::ResourceManager::createShaderProgram(QString vertexShaderFilePath, QString fragmentShaderFilePath, QString shaderName)
{
    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram();
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFilePath))
    {
        delete shader;
        return false;
    }
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFilePath))
    {
        delete shader;
        return false;
    }
    if (!shader->link())
    {
        delete shader;
        return false;
    }

    m_shaders.insert(shaderName, shader);
    return true;
}

QOpenGLShaderProgram *DrawableObjectTools::ResourceManager::getShaderProgram(QString key) const
{
    if (key.isEmpty())
    {
        return nullptr;
    }

    if (!m_shaders.contains(key))
    {
        return nullptr;
    }

    return m_shaders[key];
}

DrawableObjectTools::ResourceManager::ResourceManager()
{
    m_shaders = QMap<QString, QOpenGLShaderProgram*>();
}
