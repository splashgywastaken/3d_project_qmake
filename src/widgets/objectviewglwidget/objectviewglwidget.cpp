#include "objectviewglwidget.h"
#include "qevent.h"

#include <QMessageBox>
#include <QQuaternion>

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
    Q_UNUSED(parent);

    // Inits for singletons
    // Camera
    m_camera = Camera::getInstance();
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

QVector3D ObjectViewGLWidget::unprojectSreenPointToEye(const QPoint &position, float depth, const QMatrix4x4 &projectionMatrix, const QSize &screenSize)
{
    QPointF normalizedPosition = Camera::screenPointToNormalizedScreenPoint(position, screenSize);
    return Camera::unproject(projectionMatrix, normalizedPosition, depth);
}

void ObjectViewGLWidget::doCameraRotation(const QPoint &dstPosition)
{
    QPoint shift = dstPosition - m_screenPosition;
    float rotationSpeed = 0.1f;
    float angleX = -shift.y() * rotationSpeed;
    float angleY = -shift.x() * rotationSpeed;
    QQuaternion screenRotation = m_cameraRotation;
    QQuaternion dstRotationX = QQuaternion::fromAxisAndAngle(1, 0, 0, angleX);
    QQuaternion dstRotationY = QQuaternion::fromAxisAndAngle(0, 1, 0, angleY);
    QQuaternion dstRotation = dstRotationY * screenRotation * dstRotationX;
    m_cameraRotation = dstRotation;
    m_camera->setRotation(dstRotation);
    m_screenPosition = dstPosition;
    update();
}

void ObjectViewGLWidget::doPanningXZ(const QPoint &dstPosition)
{
    QMatrix4x4 projectionMatrix = m_camera->projectionMatrix(getAspectRatio());
    float depth = m_camera->getCameraPosition().z();
    QVector3D screenPosition3D = unprojectSreenPointToEye(m_screenPosition, depth, projectionMatrix, size());
    QVector3D dstPosition3D = unprojectSreenPointToEye(dstPosition, depth, projectionMatrix, size());

    std::swap(screenPosition3D[1], screenPosition3D[2]);
    std::swap(dstPosition3D[1], dstPosition3D[2]);

    QVector3D shift = dstPosition3D - screenPosition3D;
    m_camera->setCameraPosition(m_camera->getCameraPosition() - shift);
    m_screenPosition = dstPosition;
    update();
}

void ObjectViewGLWidget::doPanningXY(const QPoint &dstPosition)
{
    QMatrix4x4 projectionMatrix = m_camera->projectionMatrix(getAspectRatio());
    float depth = m_camera->getCameraPosition().z();
    QVector3D screenPosition3D = unprojectSreenPointToEye(m_screenPosition, depth, projectionMatrix, size());
    QVector3D dstPosition3D = unprojectSreenPointToEye(dstPosition, depth, projectionMatrix, size());
    QVector3D shift = dstPosition3D - screenPosition3D;
    m_camera->setCameraPosition(m_camera->getCameraPosition() - shift);
    m_screenPosition = dstPosition;
    update();
}

void ObjectViewGLWidget::doZooming(const QPoint &dstPosition)
{
    const QPoint diff = dstPosition - m_screenPosition;
    float shift = -diff.x() - diff.y();
    QVector3D cameraPosition = m_camera->getCameraPosition() * (1 + shift * 1e-3);
    m_camera->setCameraPosition(cameraPosition);
    m_screenPosition = dstPosition;
    update();
}

void ObjectViewGLWidget::doWheelZooming(float delta)
{
    QVector3D cameraPosition = m_camera->getCameraPosition();
    float speed = 1e-3f;
    cameraPosition += -delta * cameraPosition * speed;
    m_camera->setCameraPosition(cameraPosition);
    update();
}

void ObjectViewGLWidget::doCameraRotationAroundObject(const QPoint &dstPosition)
{
    QPoint shift = dstPosition - m_screenPosition;
    float rotationSpeed = 0.1f;
    float angleX = -shift.y() * rotationSpeed;
    float angleY = -shift.x() * rotationSpeed;
    QQuaternion screenRotation = m_cameraRotation;
    QQuaternion dstRotationX = QQuaternion::fromAxisAndAngle(1, 0, 0, angleX);
    QQuaternion dstRotationY = QQuaternion::fromAxisAndAngle(0, 1, 0, angleY);
    QQuaternion dstRotation = dstRotationY * screenRotation * dstRotationX;
    m_cameraRotation = dstRotation;
    m_camera->setRotation(dstRotation);
    m_screenPosition = dstPosition;
    update();
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

void ObjectViewGLWidget::mousePressEvent(QMouseEvent *event)
{
    bool altPressed = (event->modifiers() == Qt::AltModifier);
    bool ctrlPressed = (event->modifiers() == Qt::ControlModifier);
    bool startPanningXZ = (event->buttons() == Qt::MiddleButton) && altPressed;
    bool startPanningXY = (event->buttons() == Qt::MiddleButton) && ctrlPressed;
    bool startRotatingCamera = (event->buttons() == Qt::LeftButton) && altPressed;
    bool startRotatingAroundObject = (event->button() == Qt::LeftButton) && ctrlPressed;
    if (startPanningXZ)
    {
        m_navigationState = NavigationState::PanXZ;
        m_screenPosition = event->pos();
    }
    if (startPanningXY)
    {
        m_navigationState = NavigationState::PanXY;
        m_screenPosition = event->pos();
    }
    bool startZooming = (event->buttons() == Qt::RightButton) && altPressed;
    if (startZooming)
    {
        m_navigationState = NavigationState::Zoom;
        m_screenPosition = event->pos();
    }
    if (startRotatingCamera)
    {
        m_navigationState = NavigationState::RotateCamera;
        m_screenPosition = event->pos();
        m_cameraRotation = m_camera->getRotation();
    }
    if (startRotatingAroundObject)
    {
        m_navigationState = NavigationState::RotateAroundObject;
        m_screenPosition = event->pos();
        m_cameraRotation = m_camera->getRotation();
    }
}

void ObjectViewGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_navigationState = NavigationState::None;
}

void ObjectViewGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_navigationState == NavigationState::Zoom)
    {
        doZooming(event->pos());
    }
    if (m_navigationState == NavigationState::RotateCamera)
    {
        doCameraRotation(event->pos());
    }
    if (m_navigationState == NavigationState::PanXZ)
    {
        doPanningXZ(event->pos());
    }
    if (m_navigationState == NavigationState::PanXY)
    {
        doPanningXY(event->pos());
    }
    if (m_navigationState == NavigationState::RotateAroundObject)
    {
        doCameraRotationAroundObject(event->pos());
    }
}

void ObjectViewGLWidget::wheelEvent(QWheelEvent *event)
{
    doWheelZooming(event->angleDelta().y() / 8);
}
