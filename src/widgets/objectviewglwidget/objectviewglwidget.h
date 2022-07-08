#ifndef OBJECTVIEWGLWIDGET_H
#define OBJECTVIEWGLWIDGET_H

#include <QOpenGLPaintDevice>
#include <QProgressBar>
#include <qopenglwidget.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include <src/models/3D_obj_data/object3d.h>

#include <src/service/GlobalState.h>

enum class NavigationState
{
    Zoom,
    RotateCamera,
    RotateAroundObject,
    Pan,
    None
};

class ObjectViewGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ObjectViewGLWidget(QWidget* parent = nullptr);
    ~ObjectViewGLWidget();

    void addObject(Object3D* object);

    // Setters
    void setVertexShaderPath(QString vertexShaderPath);
    void setFragmentShaderPath(QString fragmentShaderPath);

    void setObjectColor(QVector3D objectColor);
    void setUseNormals(bool useNormals);

    // Getters
    QVector3D getObjectColor();
    float getAspectRatio() const;

protected:
    QVector3D unprojectSreenPointToEye(const QPoint &position, float depth, const QMatrix4x4 &projectionMatrix, const QSize &screenSize);

    void doCameraRotation(const QPoint &dstPosition);
    void doPanning(const QPoint &dstPosition);
    void doZooming(const QPoint &dstPosition);
    void doWheelZooming(float delta);
    void doCameraRotationAroundObject(const QPoint &dstPosition);

    // Overrides
    // QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // QWidget interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

protected:
//    QVector<Object3D>* m_objects;
    Object3D* m_object = nullptr;
    Camera* m_camera = nullptr;

    QString m_vertexShaderPath;
    QString m_fragmentShaderPath;

    int m_nVertices;

    bool m_useNormals = true;

    NavigationState m_navigationState = NavigationState::None;
    QPoint m_screenPosition;
    QQuaternion m_cameraRotation;

};

#endif // OBJECTVIEWGLWIDGET_H
