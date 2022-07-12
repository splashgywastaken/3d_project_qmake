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

#include <CameraMovementEventFilter/CameraMovemEnteventFilter.h>

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
    // Overrides
    // QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
protected:
//    QVector<Object3D>* m_objects;
    Object3D* m_object = nullptr;
    Camera* m_camera = nullptr;
    CameraMovementEventFilter* m_cameraMovementEventFilter = nullptr;
    QOpenGLShaderProgram* m_shader = nullptr;
    DrawableObjectTools::ShaderProgrammType m_shaderType = DrawableObjectTools::ShaderProgrammType::Lightning;

    int m_nVertices;
    int m_fitToViewSpacing = 5;
};

#endif // OBJECTVIEWGLWIDGET_H
