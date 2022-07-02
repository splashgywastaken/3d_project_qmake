#include "objectviewglwidget.h"

ObjectViewGLWidget::ObjectViewGLWidget(QWidget* parent)
{

    object = nullptr;

    m_program = nullptr;
    m_context = nullptr;
    m_device = nullptr;

}

ObjectViewGLWidget::~ObjectViewGLWidget()
{

}

bool ObjectViewGLWidget::generateArrays(QProgressBar *progressBar)
{
    return object->generateData(progressBar);
}

void ObjectViewGLWidget::setFileData(ObjFileData* fileData)
{
    if (object == nullptr){
        object = new Object3D("Object", fileData);
    }
    else {
        object->setFileData(fileData);
    }
}

void ObjectViewGLWidget::initializeGL()
{

}

void ObjectViewGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ObjectViewGLWidget::paintGL()
{
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);


}
