#include "camera.h"

Camera* Camera::m_instance = nullptr;

Camera::Camera()
{
    m_fovY = 25;
    m_zNear = 0.01;
    m_zFar = 1000;
    m_cameraPosition = {0, 0, 5};
}

QMatrix4x4 Camera::cameraToWorldMatrix() const
{
    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.scale(1);
    modelViewMatrix.translate(m_cameraPosition);
    modelViewMatrix.rotate(m_cameraRotation);

    return modelViewMatrix;
}

float Camera::degToRad(float deg) const
{
    return deg / 180 * M_PI;
}

Camera *Camera::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new Camera();
    }

    return m_instance;
}

QPointF Camera::screenPointToNormalizedScreenPoint(const QPoint &screenPoint, const QSize screenSize)
{
    float xNorm = 2 * (screenPoint.x()) / float(screenSize.width()) - 1;
    float yNorm = 1 - 2 * (screenPoint.y()) / float(screenSize.height());
    return QPointF(xNorm, yNorm);
}

QVector3D Camera::unproject(QMatrix4x4 projectionMatrix, const QPointF &normalizedScreenPoint, float depth)
{
    float zEye = -depth;
    float w = projectionMatrix(3, 2) * zEye + projectionMatrix(3, 3) * 1;
    const float xClip = normalizedScreenPoint.x() * w;
    const float yClip = normalizedScreenPoint.y() * w;
    const float zClip = (projectionMatrix(2, 2) * zEye + projectionMatrix(2, 3));
    QVector4D ndcPoint(xClip, yClip, zClip, w);
    bool invertible = true;
    const QMatrix4x4 projectionMatrixInverted = projectionMatrix.inverted(&invertible);
    Q_ASSERT(invertible);
    return QVector3D(projectionMatrixInverted * ndcPoint);
}

QMatrix4x4 Camera::viewMatrix() const
{
    QMatrix4x4 cameraToWorld = cameraToWorldMatrix();
    bool invertible = true;
    QMatrix4x4 worldToCamera = cameraToWorld.inverted(&invertible);
    Q_ASSERT(invertible);
    return worldToCamera;
}

QMatrix4x4 Camera::projectionMatrix(float screenAspectRatio) const
{
    float ySide = m_zNear * tan(degToRad(m_fovY) / 2);
    float xMax = ySide * screenAspectRatio;
    float yMax = ySide;
    float xMin = -xMax;
    float yMin = -yMax;
    QMatrix4x4 projectionMatrix;
    projectionMatrix.frustum(xMin, xMax, yMin, yMax, m_zNear, m_zFar);
    return projectionMatrix;
}

void Camera::setFov(float fov)
{
    m_fovY = fov;
}

void Camera::setZNear(float zNear)
{
    m_zNear = zNear;
}

void Camera::setZFar(float zFar)
{
    m_zFar = zFar;
}

void Camera::setRotation(const QQuaternion &rotation)
{
    m_cameraRotation = rotation;
}

void Camera::setCameraPosition(const QVector3D &position)
{
    m_cameraPosition = position;
}

QQuaternion Camera::getRotation() const
{
    return m_cameraRotation;
}

QVector3D Camera::getCameraPosition() const
{
    return m_cameraPosition;
}
