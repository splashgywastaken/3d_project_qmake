#ifndef DRAWABLEOBJECTTOOLS_H
#define DRAWABLEOBJECTTOOLS_H

#include "qopenglshaderprogram.h"
#include <QString>


namespace DrawableObjectTools
{

enum class ShaderProgrammType
{
    Standard,
    Lightning,
    LightningWithTextures
};

class ResourceManager
{
public:

    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;

    static ResourceManager* getInstance();

    bool createShaderProgram(QString vertexShaderFilePath, QString fragmentShaderFilePath, QString shaderName);
    QOpenGLShaderProgram* getShaderProgram(QString key) const;
protected:
    static ResourceManager* m_instance;

    QMap<QString, QOpenGLShaderProgram*> shaders;
private:
    ResourceManager();
};

}

#endif // DRAWABLEOBJECTTOOLS_H
