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

#include "src/models/3D_obj_data/object3d.h"

#include <src/service/GlobalState.h>

// TODO:: Закончить работу над шейдером, сделать возможность передачи данных о вершинах и т.д.

class ObjectViewGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ObjectViewGLWidget(QWidget* parent = nullptr);
    ~ObjectViewGLWidget();

    bool generateArrays(AbstractProgressNotifier* progressNotifier);
    void setFileData(ObjFileData *fileData);
    bool addObject(QList<GLfloat> vertices);
    bool addObject();

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram* createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);

    Object3D* object;

    QOpenGLShaderProgram *m_shader;
    QOpenGLBuffer *m_vertexBuffer;

    int m_nVertices;
};

#endif // OBJECTVIEWGLWIDGET_H
