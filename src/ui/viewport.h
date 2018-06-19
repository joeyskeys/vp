#pragma once

#include <GL/glew.h>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include <model/mesh.h>
#include <model/light.h>

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
	void clearGL();
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    QOpenGLShaderProgram *m_program;

	GLuint		m_vbo1, m_vbo2;
	GLuint		m_vao;

    GLuint      m_proj_loc;
    QMatrix4x4  m_proj_val;
    GLuint      m_view_loc;
    QMatrix4x4  m_view_val;
    GLuint      m_light_position_loc;
    GLuint      m_light_color_loc;

    Mesh        m_mesh;
	Light		m_light;
};
