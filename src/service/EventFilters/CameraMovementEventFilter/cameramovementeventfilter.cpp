#include "cameramovementeventfilter.h"

CameraMovementEventFilter::CameraMovementEventFilter(Camera* camera)
{
    m_camera = camera;
    m_navigationState = NavigationState::None;
}

bool CameraMovementEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    m_openGlWidget = static_cast<QOpenGLWidget*>(watched);
    if (event->type() == QEvent::MouseButtonPress)
    {
        static QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        mousePressEvent(mouseEvent);
    #ifndef QT_NO_DEBUG
//       qDebug() << "mouse button pressed";
    #endif
        return true;
    }
    if (event->type() == QEvent::MouseMove)
    {
        static QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        mouseMoveEvent(mouseEvent);
    #ifndef QT_NO_DEBUG
//        qDebug() << "mouse moved";
    #endif
        return true;
    }
    if (event->type() == QEvent::MouseButtonRelease)
    {
        static QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        mouseReleaseEvent(mouseEvent);
    #ifndef QT_NO_DEBUG
//        qDebug() << "mouse button released";
    #endif
        return true;
    }
    if (event->type() == QEvent::Wheel)
    {
        static QWheelEvent* mouseWheelEvent = static_cast<QWheelEvent*>(event);
        wheelEvent(mouseWheelEvent);
    #ifndef QT_NO_DEBUG
//        qDebug() << "wheel event";
    #endif
        return true;
    }
    return false;
}

void CameraMovementEventFilter::mousePressEvent(QMouseEvent *event)
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

void CameraMovementEventFilter::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_navigationState = NavigationState::None;
}

void CameraMovementEventFilter::mouseMoveEvent(QMouseEvent *event)
{
    switch (m_navigationState)
    {
        case NavigationState::Zoom:
            {
                doZooming(event->pos());
            }
            break;

        case NavigationState::RotateCamera:
            {
                doCameraRotation(event->pos());
            }
            break;

        case NavigationState::RotateAroundObject:
            {
                doCameraRotationAroundObject(event->pos());
            }
            break;

        case NavigationState::PanXZ:
            {
                doPanningXZ(event->pos());
            }
            break;

        case NavigationState::PanXY:
            {
                doPanningXY(event->pos());
            }
            break;

        case NavigationState::None:
        default:
            break;
    }
}

void CameraMovementEventFilter::wheelEvent(QWheelEvent *event)
{
    doWheelZooming(event->angleDelta().y() / 8);
}

QVector3D CameraMovementEventFilter::unprojectSreenPointToEye(const QPoint &position, float depth, const QMatrix4x4 &projectionMatrix, const QSize &screenSize)
{
    QPointF normalizedPosition = Camera::screenPointToNormalizedScreenPoint(position, screenSize);
    return Camera::unproject(projectionMatrix, normalizedPosition, depth);
}

float CameraMovementEventFilter::getAspectRatio() const
{
    return m_openGlWidget->width() / float(m_openGlWidget->height());
}

void CameraMovementEventFilter::doCameraRotation(const QPoint &dstPosition)
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

void CameraMovementEventFilter::doPanningXZ(const QPoint &dstPosition)
{
    QMatrix4x4 projectionMatrix = m_camera->projectionMatrix(getAspectRatio());
    float depth = m_camera->getCameraPosition().z();
    QVector3D screenPosition3D = unprojectSreenPointToEye(m_screenPosition, depth, projectionMatrix, m_openGlWidget->size());
    QVector3D dstPosition3D = unprojectSreenPointToEye(dstPosition, depth, projectionMatrix, m_openGlWidget->size());

    std::swap(screenPosition3D[1], screenPosition3D[2]);
    std::swap(dstPosition3D[1], dstPosition3D[2]);

    QVector3D shift = dstPosition3D - screenPosition3D;
    m_camera->setCameraPosition(m_camera->getCameraPosition() - shift);
    m_screenPosition = dstPosition;
    update();
}

void CameraMovementEventFilter::doPanningXY(const QPoint &dstPosition)
{
    QMatrix4x4 projectionMatrix = m_camera->projectionMatrix(getAspectRatio());
    float depth = m_camera->getCameraPosition().z();
    QVector3D screenPosition3D = unprojectSreenPointToEye(m_screenPosition, depth, projectionMatrix, m_openGlWidget->size());
    QVector3D dstPosition3D = unprojectSreenPointToEye(dstPosition, depth, projectionMatrix, m_openGlWidget->size());
    QVector3D shift = dstPosition3D - screenPosition3D;
    m_camera->setCameraPosition(m_camera->getCameraPosition() - shift);
    m_screenPosition = dstPosition;
    update();
}

void CameraMovementEventFilter::doZooming(const QPoint &dstPosition)
{
    const QPoint diff = dstPosition - m_screenPosition;
    float shift = -diff.x() - diff.y();
    QVector3D cameraPosition = m_camera->getCameraPosition() * (1 + shift * 1e-3);
    m_camera->setCameraPosition(cameraPosition);
    m_screenPosition = dstPosition;
    update();
}

void CameraMovementEventFilter::doWheelZooming(float delta)
{
    QVector3D cameraPosition = m_camera->getCameraPosition();
    float speed = 1e-3f;
    cameraPosition += -delta * cameraPosition * speed;
    m_camera->setCameraPosition(cameraPosition);
    update();
}

// TODO:: Work more on this thing
void CameraMovementEventFilter::doCameraRotationAroundObject(const QPoint &dstPosition)
{
    QVector3D cameraPositionNormalized = m_camera->getCameraPosition().normalized();
    QPoint shift = dstPosition - m_screenPosition;
    float rotationSpeed = 0.1f;
    float angleY = -shift.x() * rotationSpeed;
    QQuaternion screenRotation = m_cameraRotation;
    QQuaternion dstRotationY = QQuaternion::fromAxisAndAngle(0, cameraPositionNormalized.y(), 0, angleY);
    QQuaternion dstRotation = dstRotationY * screenRotation;
    QVector3D cameraPosition = dstRotationY * m_camera->getCameraPosition();
    m_cameraRotation = dstRotation;
    m_camera->setRotation(dstRotation);
    m_camera->setCameraPosition(cameraPosition);
    m_screenPosition = dstPosition;
    update();
}

void CameraMovementEventFilter::update()
{
    m_openGlWidget->update();
}
