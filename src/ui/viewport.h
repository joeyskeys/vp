#pragma once

#include <GL/glew.h>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include <model/mesh.h>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class QtViewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit QtViewport(QWidget *parent = 0);
    ~QtViewport();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo;

    GLuint      m_proj_loc;
    QMatrix4x4  m_proj_val;
    GLuint      m_view_loc;
    QMatrix4x4  m_view_val;

    Mesh        m_mesh;
};
