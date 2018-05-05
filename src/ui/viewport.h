#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class QtViewport : public QOpenGLWidget, protected QOpenGLFunctions
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
};
