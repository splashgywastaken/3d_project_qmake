#include "objectviewglwidget.h"

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
    object = nullptr;
}

ObjectViewGLWidget::~ObjectViewGLWidget()
{

}

bool ObjectViewGLWidget::generateArrays(AbstractProgressNotifier* progressNotifier = nullptr)
{
    return object->generateData(progressNotifier);
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

bool ObjectViewGLWidget::addObject(QList<GLfloat> vertices)
{
    // Добавить триангуляцию граней объекта
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    // Create vertex buffer
    if (!m_vertexBuffer->create()){
        return false;
    }
    // Made buffer active
    if (!m_vertexBuffer->bind()){
        return false;
    }

    // Add data for a buffer
    m_nVertices = vertices.size() / 3;
    int dataSize = vertices.size() * sizeof(float);
    m_vertexBuffer->allocate(vertices.constData(), dataSize);
    m_vertexBuffer->release();
    return true;
}

bool ObjectViewGLWidget::addObject()
{
    return addObject(*object->getObjectArrays()->take("vertices"));
}

void ObjectViewGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    QString vertexShaderFileName = "E:/projects SSD/Qt/3d_project_qmake/src/widgets/objectviewglwidget/shaders/basicShader/basicShader.vsh";
    QString fragmentShaderFileName = "E:/projects SSD/Qt/3d_project_qmake/src/widgets/objectviewglwidget/shaders/basicShader/basicShader.fsh";

    m_shader = createShaderProgram(vertexShaderFileName, fragmentShaderFileName);
    Q_ASSERT(m_shader != nullptr);

    QList<GLfloat> vertices;
    vertices.append(0.0f);
    vertices.append(0.0f);
    vertices.append(0.0f);
    addObject(vertices);
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

    if (!m_shader->bind())
        qFatal("Can not bind shader");
    m_shader->setUniformValue("modelViewMatrix", modelViewMatrix);
    m_shader->setUniformValue("projectionMatrix", projectionMatrix);

    if(!m_vertexBuffer->bind())
        qFatal("Can not bind shader");
    m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    m_shader->enableAttributeArray("vertex");
    m_vertexBuffer->release();

    glDrawArrays(GL_LINES, 0, m_nVertices);

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
