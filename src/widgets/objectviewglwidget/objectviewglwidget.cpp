#include "objectviewglwidget.h"
#include "qevent.h"

#include <QMessageBox>
#include <QQuaternion>

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
    Q_UNUSED(parent);

    // Inits for fields
    // Camera
    m_camera = Camera::getInstance();
    // Camera movement filter:
    m_cameraMovementEventFilter = new CameraMovementEventFilter(m_camera);
    installEventFilter(m_cameraMovementEventFilter);
}

ObjectViewGLWidget::~ObjectViewGLWidget()
{
}

void ObjectViewGLWidget::addObject(Object3D *object)
{
//    m_objects.append(object);
    m_object = object;
}

void ObjectViewGLWidget::setObjectColor(QVector3D objectColor)
{
    if (m_object != nullptr)
    {
        m_object->setObjectColor(objectColor.normalized());
    }
}

void ObjectViewGLWidget::switchShaders(DrawableObjectTools::ShaderProgrammType shaderType)
{
    m_shaderType = shaderType;

    DrawableObjectTools:: ResourceManager *resourceManager = DrawableObjectTools::ResourceManager::getInstance();

    if (m_shaderType == DrawableObjectTools::ShaderProgrammType::Standard)
    {
        m_shader = resourceManager->getShaderProgram("basicShader");
    }
    if (m_shaderType == DrawableObjectTools::ShaderProgrammType::Lightning)
    {
        m_shader = resourceManager->getShaderProgram("lightningShader");
    }
    if (m_shaderType == DrawableObjectTools::ShaderProgrammType::NormalMap)
    {
        m_shader = resourceManager->getShaderProgram("normalMapShader");
    }

    Q_ASSERT(m_shader != nullptr);
}

QVector3D ObjectViewGLWidget::getObjectColor()
{
    if (m_object != nullptr)
    {
        return m_object->getObjectColor();
    }
    return QVector3D();
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
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create mv matrix
    QMatrix4x4 modelViewMatrix = m_camera->viewMatrix();

    // Create projection matrix
    QMatrix4x4 projectionMatrix = m_camera->projectionMatrix(getAspectRatio());

//    for (Object3D * object3D : m_objects)
//    {
//        object3D->draw(modelViewMatrix, projectionMatrix);
//    }

    if (m_object != nullptr)
    {
        m_object->draw(modelViewMatrix, projectionMatrix, m_shader, m_shaderType);
    }

}
