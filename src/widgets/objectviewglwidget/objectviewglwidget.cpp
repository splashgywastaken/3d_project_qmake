#include "objectviewglwidget.h"

#include <QMessageBox>

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
}

ObjectViewGLWidget::~ObjectViewGLWidget()
{
}

void ObjectViewGLWidget::addObject(Object3D *object)
{
//    m_objects.append(object);
    m_object = object;
}

void ObjectViewGLWidget::setVertexShaderPath(QString vertexShaderPath)
{
    m_vertexShaderPath = vertexShaderPath;
}

void ObjectViewGLWidget::setFragmentShaderPath(QString fragmentShaderPath)
{
    m_fragmentShaderPath = fragmentShaderPath;
}

void ObjectViewGLWidget::setObjectColor(QVector3D objectColor)
{
    m_object->setObjectColor(objectColor.normalized());
}

void ObjectViewGLWidget::setUseNormals(bool useNormals)
{
    m_useNormals = useNormals;
}

QVector3D ObjectViewGLWidget::getObjectColor()
{
    return m_object->getObjectColor();
}

void ObjectViewGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
}

void ObjectViewGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ObjectViewGLWidget::paintGL()
{
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create mv matrix
    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.scale(1);
    modelViewMatrix.translate(0, 0, -5);

    // Create projection matrix
    QMatrix4x4 projectionMatrix;
    Q_ASSERT((width() != 0) && (height() != 0));
    float screenAspectRatio = width() / float(height());
    projectionMatrix.ortho(-1, 1, -1 / screenAspectRatio, 1 / screenAspectRatio, 0.5, 5);

//    for (Object3D * object3D : m_objects)
//    {
//        object3D->draw(modelViewMatrix, projectionMatrix);
//    }

    if (m_object != nullptr)
    {
        m_object->draw(modelViewMatrix, projectionMatrix);
    }

}

QOpenGLShaderProgram *ObjectViewGLWidget::createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath)
{
    QOpenGLShaderProgram *shader = new QOpenGLShaderProgram();
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath)){
        delete shader;
        return nullptr;
    }

    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)){
        delete shader;
        return nullptr;
    }

    if (!shader->link()){
        delete shader;
        return nullptr;
    }

    return shader;
}
