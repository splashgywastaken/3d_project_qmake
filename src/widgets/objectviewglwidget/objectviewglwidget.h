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

#include <src/models/3D_obj_data/Object3D/object3d.h>

#include <src/service/GlobalState.h>

enum class NavigationState
{
    Zoom,
    RotateCamera,
    RotateAroundObject,
    PanXZ,
    PanXY,
    None
};

class ObjectViewGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ObjectViewGLWidget(QWidget* parent = nullptr);
    ~ObjectViewGLWidget();

    void addObject(Object3D* object);

    void setObjectColor(QVector3D objectColor);
    void setUseNormals(bool useNormals);

    void switchShaders(DrawableObjectTools::ShaderProgrammType shaderType);

    // Getters
    QVector3D getObjectColor();
    float getAspectRatio() const;

protected:
    QVector3D unprojectSreenPointToEye(const QPoint &position, float depth, const QMatrix4x4 &projectionMatrix, const QSize &screenSize);

    void doCameraRotation(const QPoint &dstPosition);
    void doPanningXZ(const QPoint &dstPosition);
    void doPanningXY(const QPoint &dstPosition);
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
    QOpenGLShaderProgram* m_shader;
    DrawableObjectTools::ShaderProgrammType m_shaderType = DrawableObjectTools::ShaderProgrammType::Lightning;

    int m_nVertices;
    int m_fitToViewSpacing = 5;

    NavigationState m_navigationState = NavigationState::None;
    QPoint m_screenPosition;
    QQuaternion m_cameraRotation;

};

#endif // OBJECTVIEWGLWIDGET_H
