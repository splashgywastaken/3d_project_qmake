#include "objectviewglwidget.h"

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
    object = nullptr;
}

ObjectViewGLWidget::~ObjectViewGLWidget()
{

}

bool ObjectViewGLWidget::generateArrays()
{
    return object->generateData();
}

void ObjectViewGLWidget::setFileData(ObjFileData* fileData)
{
    if (object == nullptr){
        object = new Object3D("Object", fileData);
    }
    else {
        object->setFileData(fileData);
    }
}

void ObjectViewGLWidget::initializeGL()
{
    QString vertexShaderFileName = "/src/shader/basicShader.vsh";
    QString fragmentShaderFileName = "/src/shader/basicShader.fsh";

    m_shader = createShaderProgram(vertexShaderFileName, fragmentShaderFileName);
    Q_ASSERT(m_shader != nullptr);
}

void ObjectViewGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ObjectViewGLWidget::paintGL()
{
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Create mv matrix
    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.scale(1);
    modelViewMatrix.translate(0, 0, -1);

    // Create projection matrix
    QMatrix4x4 projectionMatrix;
    Q_ASSERT((width() != 0) && (height() != 0));
    float screenAspectRatio = width() / float(height());
    projectionMatrix.ortho(-1, 1, -1 / screenAspectRatio, 1 / screenAspectRatio, 0.5, 5);

    if (!m_shader->bind())
        qFatal("Can not bind shader");
    m_shader->setUniformValue("modelViewMatrix", modelViewMatrix);
    m_shader->setUniformValue("projectionMatrix", projectionMatrix);

    if(!m_vertexBuffer->bind())
        qFatal("Can not bind shader");
    m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    m_shader->enableAttributeArray("vertex");
    m_vertexBuffer->release();

    glDrawArrays(GL_TRIANGLES, 0, m_nVertices);

    m_shader->release();
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
