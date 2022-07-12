#ifndef CAMERAMOVEMENTEVENTFILTER_H
#define CAMERAMOVEMENTEVENTFILTER_H

#include "qevent.h"
#include <QObject>
#include <QOpenGLWidget>
#include <camera.h>


class CameraMovementEventFilter : public QObject
{
    Q_OBJECT

protected:
enum class NavigationState
{
    Zoom,
    RotateCamera,
    RotateAroundObject,
    PanXZ,
    PanXY,
    None
};

public:
    CameraMovementEventFilter(Camera* camera);

    // QObject interface
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    QVector3D unprojectSreenPointToEye(const QPoint &position, float depth, const QMatrix4x4 &projectionMatrix, const QSize &screenSize);
    float getAspectRatio() const;

    void doCameraRotation(const QPoint &dstPosition);
    void doPanningXZ(const QPoint &dstPosition);
    void doPanningXY(const QPoint &dstPosition);
    void doZooming(const QPoint &dstPosition);
    void doWheelZooming(float delta);
    void doCameraRotationAroundObject(const QPoint &dstPosition);

protected:
    Camera* m_camera;
    QOpenGLWidget* m_openGlWidget;

    NavigationState m_navigationState;
    QPoint m_screenPosition;
    QQuaternion m_cameraRotation;
};

#endif // CAMERAMOVEMENTEVENTFILTER_H
