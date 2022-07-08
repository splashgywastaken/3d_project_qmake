#include "objectviewglwidget.h"
#include "qevent.h"

#include <QMessageBox>
#include <QQuaternion>

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{
    Q_UNUSED(parent);
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

void ObjectViewGLWidget::doPanning(const QPoint &dstPosition)
{
    QMatrix4x4 projectionMatrix = m_camera->projectionMatrix(getAspectRatio());
    float depth = m_camera->getCameraPosition().z();
    QVector3D screenPosition3D = unprojectSreenPointToEye(m_screenPosition, depth, projectionMatrix, size());
    QVector3D destinationPosition3D = unprojectSreenPointToEye(dstPosition, depth, projectionMatrix, size());
    QVector3D shift = destinationPosition3D - screenPosition3D;
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
        m_object->draw(modelViewMatrix, projectionMatrix);
    }

}

void ObjectViewGLWidget::mousePressEvent(QMouseEvent *event)
{
    bool altPressed = (event->modifiers() == Qt::AltModifier);
    bool ctrlPressed = (event->modifiers() == Qt::ControlModifier);
    bool startPanning = (event->buttons() == Qt::MiddleButton) && altPressed;
    bool startRotatingCamera = (event->buttons() == Qt::LeftButton) && altPressed;
    bool startRotatingAroundObject = (event->button() == Qt::LeftButton) && ctrlPressed;
    if (startPanning)
    {
        m_navigationState = NavigationState::Pan;
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
    if (m_navigationState == NavigationState::Pan)
    {
        doPanning(event->pos());
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
