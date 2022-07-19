#include "objectviewglwidget.h"
#include "qevent.h"

#include <QMessageBox>
#include <QQuaternion>

#include <Grid/grid.h>

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
    Q_UNUSED(parent);

    // Inits for fields
    // Camera
    m_camera = Camera::getInstance();
    // Camera movement filter:
    m_cameraMovementEventFilter = new CameraMovementEventFilter(m_camera);
    installEventFilter(m_cameraMovementEventFilter);

    m_grid = new Grid(3.0f, 10, QVector3D(0.9, 0.9, 0.9));
}

ObjectViewGLWidget::~ObjectViewGLWidget()
{
}

void ObjectViewGLWidget::addObject(SceneObject* object)
{
    if (m_objects == nullptr)
    {
        m_objects = new QVector<SceneObject*>();
    }
    m_objects->append(object);
}

void ObjectViewGLWidget::addPoint(QVector3D point)
{
    if (m_selectedPoints != nullptr)
    {
        delete m_selectedPoints;
        m_selectedPoints = nullptr;
    }
    m_selectedPoints = new QVector<QVector3D>();
    m_selectedPoints->append(point);
}

void ObjectViewGLWidget::deleteLastObject()
{
    if (m_objects != nullptr)
    {
        m_objects->pop_back();
    }
}

void ObjectViewGLWidget::clearObjects()
{
    if (m_objects != nullptr)
    {
        m_objects->clear();
        delete m_objects;
        m_objects = nullptr;
    }
    if (m_selectedPoints != nullptr)
    {
        m_selectedPoints->clear();
        delete m_selectedPoints;
        m_selectedPoints = nullptr;
    }
}

void ObjectViewGLWidget::removeObject(SceneObject *object)
{
    int index = m_objects->indexOf(object);
    Q_ASSERT(index != -1);
    m_objects->removeAt(index);
}

void ObjectViewGLWidget::setObjectColor(QVector3D objectColor)
{
    if (m_objects != nullptr)
    {
        if (m_objects->count() != 0)
        {
            m_objects->last()->setObjectColor(objectColor.normalized());
        }
    }
}

QVector3D ObjectViewGLWidget::getObjectColor()
{
    if (m_objects != nullptr)
    {
        if (m_objects->count() != 0)
        {
            return m_objects->last()->getObjectColor();
        }
    }
    return QVector3D(1, 0, 0);
}

void ObjectViewGLWidget::switchShaders(DrawableObjectTools::ShaderProgrammType shaderType)
{
    m_shaderType = shaderType;

    DrawableObjectTools:: ResourceManager *resourceManager = DrawableObjectTools::ResourceManager::getInstance();

    switch (m_shaderType)
    {
        case DrawableObjectTools::ShaderProgrammType::Standard:
            {
                m_shader = resourceManager->getShaderProgram("basicShader");
            }
            break;
        case DrawableObjectTools::ShaderProgrammType::Lightning:
            {
                m_shader = resourceManager->getShaderProgram("lightningShader");
            }
            break;
        case DrawableObjectTools::ShaderProgrammType::LightningWithTextures:
            {
            }
            break;
        case DrawableObjectTools::ShaderProgrammType::NormalMap:
            {
                m_shader = resourceManager->getShaderProgram("normalMapShader");
            }
            break;
    }

    Q_ASSERT(m_shader != nullptr);
}

void ObjectViewGLWidget::qglClearColor(QColor color)
{
    glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

float ObjectViewGLWidget::getAspectRatio() const
{
    return width() / float(height());
}

void ObjectViewGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // init for resource manager
    DrawableObjectTools::ResourceManager* resourceManager = DrawableObjectTools::ResourceManager::getInstance();

    QString vertexShaderPath = "E:/projects SSD/Qt/3d_project_qmake/res/shaders/basicShader/basicShader.vert";
    QString fragmentShaderPath = "E:/projects SSD/Qt/3d_project_qmake/res/shaders/basicShader/basicShader.frag";
    Q_ASSERT(resourceManager->createShaderProgram(vertexShaderPath, fragmentShaderPath, "basicShader"));

    vertexShaderPath = "E:/projects SSD/Qt/3d_project_qmake/res/shaders/lightningShader/lightningShader.vert";
    fragmentShaderPath = "E:/projects SSD/Qt/3d_project_qmake/res/shaders/lightningShader/lightningShader.frag";
    Q_ASSERT(resourceManager->createShaderProgram(vertexShaderPath, fragmentShaderPath, "lightningShader"));

    vertexShaderPath = "E:/projects SSD/Qt/3d_project_qmake/res/shaders/normalMapShader/normalMapShader.vert";
    fragmentShaderPath = "E:/projects SSD/Qt/3d_project_qmake/res/shaders/normalMapShader/normalMapShader.frag";
    Q_ASSERT(resourceManager->createShaderProgram(vertexShaderPath, fragmentShaderPath, "normalMapShader"));

    switchShaders(m_shaderType);
}

void ObjectViewGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ObjectViewGLWidget::paintGL()
{
    qglClearColor(m_backgroundColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create mv matrix
    QMatrix4x4 modelViewMatrix = m_camera->viewMatrix();

    // Create projection matrix
    QMatrix4x4 projectionMatrix = m_camera->projectionMatrix(getAspectRatio());

    m_grid->draw(modelViewMatrix, projectionMatrix, m_shader, m_shaderType);
    if (m_objects != nullptr)
    {
        for (SceneObject* object3D : *m_objects)
        {
            object3D->draw(modelViewMatrix, projectionMatrix, m_shader, m_shaderType);
        }
    }
    drawPoints(modelViewMatrix, projectionMatrix);
}

void ObjectViewGLWidget::drawPoints(QMatrix4x4 mvMatrix, QMatrix4x4 projectionMatrix)
{
    if (m_selectedPoints != nullptr)
    {
        for (QVector3D point : *m_selectedPoints)
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(mvMatrix.data());
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(projectionMatrix.data());
            glColor3f(1, 0.6471, 0.0);
            glPointSize(10.0f);
            glBegin(GL_POINTS);
                glVertex3f(point.x(), point.y(), point.z());
            glEnd();
        }
    }
}
