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

    shaders.insert(shaderName, shader);
    return true;
}

QOpenGLShaderProgram *DrawableObjectTools::ResourceManager::getShaderProgram(QString key) const
{
    if (key.isEmpty())
    {
        return nullptr;
    }

    if (!shaders.contains(key))
    {
        return nullptr;
    }

    return shaders[key];
}

DrawableObjectTools::ResourceManager::ResourceManager()
{
    shaders = QMap<QString, QOpenGLShaderProgram*>();
}
