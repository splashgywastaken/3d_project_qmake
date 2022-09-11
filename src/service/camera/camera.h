#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>


// Camera singleton
class Camera
{
public:
    static Camera* getInstance();

    static QPointF screenPointToNormalizedScreenPoint(const QPoint &screenPoint, const QSize screenSize);
    static QVector3D unproject(QMatrix4x4 projectionMatrix, const QPointF &normalizedScreenPoint, float depth);

    QMatrix4x4 viewMatrix() const;
    QMatrix4x4 projectionMatrix(float aspectRatio) const;

    void setFov(float fov);
    void setZNear(float zNear);
    void setZFar(float zFar);
    void setRotation(const QQuaternion &rotation);
    void setCameraPosition(const QVector3D &position);

    QQuaternion getRotation() const;
    QVector3D getCameraPosition() const;

protected:
    static Camera* m_instance;

    float m_fovY;
    float m_zNear;
    float m_zFar;

    QVector3D m_cameraPosition;
    QQuaternion m_cameraRotation;
protected:
    Camera();

    QMatrix4x4 cameraToWorldMatrix() const;
    float degToRad(float deg) const;
};

#endif // CAMERA_H
