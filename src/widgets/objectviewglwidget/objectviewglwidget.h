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

class ObjectViewGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ObjectViewGLWidget(QWidget* parent = nullptr);
    ~ObjectViewGLWidget();

    void addObject(Object3D* object);

    void setVertexShaderPath(QString vertexShaderPath);
    void setFragmentShaderPath(QString fragmentShaderPath);

    void setObjectColor(QVector3D objectColor);
    void setUseNormals(bool useNormals);

    QVector3D getObjectColor();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:


    QOpenGLShaderProgram* createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);

//    QVector<Object3D>* m_objects;
    Object3D* m_object = nullptr;

    QString m_vertexShaderPath;
    QString m_fragmentShaderPath;

    int m_nVertices;

    bool m_useNormals = true;
};

#endif // OBJECTVIEWGLWIDGET_H
