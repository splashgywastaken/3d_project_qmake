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

    void reinit();

    bool generateArrays(ObjFileData& fileData, AbstractProgressNotifier* progressNotifier);
    bool addObject();

    void setVertexShaderPath(QString vertexShaderPath);
    void setFragmentShaderPath(QString fragmentShaderPath);

    void setObjectColor(QVector3D objectColor);
    void setUseNormals(bool useNormals);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    bool addObject(QList<GLfloat> vertices);
    bool addObject(QList<GLfloat> vertices, QList<GLfloat> normals);

    QOpenGLShaderProgram* createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);

    Object3D m_object;

    QVector3D m_objectColor = QVector3D(0.0, 1.0, 1.0);

    QOpenGLShaderProgram *m_shader;
    QOpenGLBuffer *m_vertexBuffer;

    QString m_vertexShaderPath;
    QString m_fragmentShaderPath;

    int m_nVertices;

    bool m_useNormals = true;
};

#endif // OBJECTVIEWGLWIDGET_H
