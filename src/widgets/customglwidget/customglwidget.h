#ifndef CUSTOMGLWIDGET_H
#define CUSTOMGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include "src/models/3D_obj_data/logo.h"
#include "src/models/3D_obj_data/object3d.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

class CustomGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CustomGLWidget(QWidget* parent = nullptr);
    ~CustomGLWidget();

    static bool isTransparent() { return CustomGLWidget::m_transparent; }
    static void setTransparent(bool t) { CustomGLWidget::m_transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setVertexArrays(QMap<QString, QList<GLfloat>*>* vertexArrays);

public slots:
    void cleanup();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setupVertexAttributes();

    bool m_core;
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QPoint m_lastPos;
    Logo m_logo;
    Object3D* m_objectData;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program = nullptr;
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;
    int m_normalMatrixLoc = 0;
    int m_lightPosLoc = 0;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    QMap<QString, QList<GLfloat>*>* m_vertexArrays;

    static bool m_transparent;
};

#endif // CUSTOMGLWIDGET_H
