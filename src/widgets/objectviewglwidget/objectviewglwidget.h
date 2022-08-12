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
#include <QThread>
#include <QOffscreenSurface>

#include <src/models/3D_obj_data/Object3D/object3d.h>
#include <src/service/GlobalState.h>
#include <src/service/EventFilters/CameraMovementEventFilter/cameramovementeventfilter.h>

class CameraMovementEventFilter;

class ObjectViewGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ObjectViewGLWidget(QWidget* parent = nullptr);
    ~ObjectViewGLWidget();

    // Add methods
    void addObject(SceneObject* object);
    void addPoint(QVector3D point);

    // Remove\delete methods
    void deleteLastObject();
    void clearObjects();
    void removeObject(SceneObject* object);

    void setObjectColor(const QColor& color);
    void setBackgroundColor(const QColor& color);
    void setGridColor(const QColor& color);
    void setUseNormals(bool useNormals);

    void switchShaders(DrawableObjectTools::ShaderProgrammType shaderType);
    void qglClearColor(QColor color);

    // Getters
    QColor getObjectColor();
    QColor getBackgroundColor();
    QColor getGridColor();

    float getAspectRatio() const;

protected:
    // Overrides
    // QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void drawPoints(QMatrix4x4 mvMatrix, QMatrix4x4 projectionMatrix);
protected:
    QVector<QVector3D>* m_selectedPoints = nullptr;
    QVector<SceneObject*>* m_objects = nullptr;
    SceneObject* m_grid = nullptr;
    Camera* m_camera = nullptr;
    CameraMovementEventFilter* m_cameraMovementEventFilter = nullptr;
    QOpenGLShaderProgram* m_shader = nullptr;
    DrawableObjectTools::ShaderProgrammType m_shaderType = DrawableObjectTools::ShaderProgrammType::Lightning;
    QColor m_backgroundColor = QColor(54, 57, 63);

    int m_nVertices;
    int m_fitToViewSpacing = 5;
};

#endif // OBJECTVIEWGLWIDGET_H
