#include "objectviewglwidget.h"

#include <QMessageBox>

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
    m_object = Object3D();
}

ObjectViewGLWidget::~ObjectViewGLWidget()
{
}

bool ObjectViewGLWidget::generateArrays(ObjFileData& fileData, AbstractProgressNotifier* progressNotifier = nullptr)
{
    return m_object.generateData(fileData, progressNotifier);
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
    return addObject(m_object.getObjectArrays().take("vertices"));
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
    m_objectColor = objectColor.normalized();
    update();
}

void ObjectViewGLWidget::setUseNormals(bool useNormals)
{
    m_useNormals = useNormals;
}

void ObjectViewGLWidget::reinit()
{
    if (m_fragmentShaderPath.isEmpty() || m_vertexShaderPath.isEmpty()){
        QString basicVertexShaderFileName = "E:/projects SSD/Qt/3d_project_qmake/src/widgets/objectviewglwidget/shaders/basicShader/basicShader.vsh";
        QString basicFragmentShaderFileName = "E:/projects SSD/Qt/3d_project_qmake/src/widgets/objectviewglwidget/shaders/basicShader/basicShader.fsh";
        QMessageBox::warning(
                    this,
                    "Не был задан вершинный или фрагментный шейдер",
                    "Программа будет использовать стандартный шейдер"
                    );
        m_shader = createShaderProgram(basicVertexShaderFileName, basicFragmentShaderFileName);
    } else
    {
        m_shader = createShaderProgram(m_vertexShaderPath, m_fragmentShaderPath);
    }
    Q_ASSERT(m_shader != nullptr);
}

void ObjectViewGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    QString basicVertexShaderFileName = "E:/projects SSD/Qt/3d_project_qmake/src/widgets/objectviewglwidget/shaders/basicShader/basicShader.vsh";
    QString basicFragmentShaderFileName = "E:/projects SSD/Qt/3d_project_qmake/src/widgets/objectviewglwidget/shaders/basicShader/basicShader.fsh";

    m_shader = createShaderProgram(basicVertexShaderFileName, basicFragmentShaderFileName);
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
    m_shader->setUniformValue("u_objectColor", m_objectColor);

    if(!m_vertexBuffer->bind())
        qFatal("Can not bind shader");
    m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    m_shader->enableAttributeArray("vertex");
    m_vertexBuffer->release();

//    glDrawArrays(GL_LINES, 0, m_nVertices);
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
