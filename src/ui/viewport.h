#pragma once

#include <GL/glew.h>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <model/mesh.h>
#include <model/light.h>
#include <model/shader.h>
#include <model/camera.h>

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

	void keyPressEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:
	GLuint		m_prog;
	ShaderProgram *m_program;
	Camera		*m_camera;

	GLuint		m_vbo1, m_vbo2;
	GLuint		m_vao;

    GLuint      m_proj_loc;
	glm::mat4	m_proj_val;
    GLuint      m_view_loc;
	glm::mat4	m_view_val;
    GLuint      m_light_position_loc;
    GLuint      m_light_color_loc;

    Mesh        m_mesh;
	Light		m_light;

	glm::vec2	m_mouse_pos;
	bool		m_move_enabled;
	bool		m_rotate_enabled;
};
