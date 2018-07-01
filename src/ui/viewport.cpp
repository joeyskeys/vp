#include "viewport.h"

#include <QOpenGLShaderProgram>
#include <QKeyEvent>

#include <cstring>

QtViewport::QtViewport(QWidget *parent) :
    QOpenGLWidget(parent),
    m_prog(0)
{
    m_mesh.fillTriangle();
	m_light.setType(LIGHT_PNT);
	m_light.setPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	m_light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_program = new ShaderProgram;
	m_camera = new Camera;
}

QtViewport::~QtViewport()
{
    makeCurrent();

    delete m_program;
	delete m_camera;

	clearGL();

    doneCurrent();
}

void QtViewport::initializeGL()
{
	if (glewInit() != GLEW_OK)
		throw std::exception("Failed to initialize GLEW\n");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	m_program->load("G:/WorkSpace/repos/vp/src/shaders/", "basic");
	m_prog = m_program->getProgram();

	m_proj_loc = glGetUniformLocation(m_prog, "proj");
	m_view_loc = glGetUniformLocation(m_prog, "view");
	m_light_position_loc = glGetUniformLocation(m_prog, "light.position");
	m_light_color_loc = glGetUniformLocation(m_prog, "light.color");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo1);
	glBufferData(GL_ARRAY_BUFFER, m_mesh.getVertSize(), m_mesh.getVerts(), GL_STATIC_DRAW);
	glGenBuffers(1, &m_vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glBufferData(GL_ARRAY_BUFFER, m_mesh.getNormSize(), m_mesh.getNorms(), GL_STATIC_DRAW);
}

void QtViewport::clearGL()
{
	glDeleteBuffers(1, &m_vbo1);
	glDeleteBuffers(1, &m_vbo2);
	glDeleteBuffers(1, &m_vao);
	//if (m_prog)
	//	glDeleteProgram(m_prog);
}

void QtViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_prog);

	m_proj_val = glm::perspective(1.047f, 4.f / 3.f, 1.f, 100.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(m_proj_val));
	//m_view_val = glm::mat4(1.f);
	//glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(m_view_val));
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_camera->getViewMatrixPtr());
	glUniform3fv(m_light_position_loc, 1, (GLfloat*)m_light.getData());

    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
	
}

void QtViewport::resizeGL(int width, int height)
{
    return;
}

void QtViewport::keyPressEvent(QKeyEvent *event)
{
	int key = event->key();

	if (key == Qt::Key_W)
		m_camera->translate(glm::vec3(0.f, 0.f, -0.2f));
	else if (key == Qt::Key_S)
		m_camera->translate(glm::vec3(0.f, 0.f, 0.2f));
	else if (key == Qt::Key_A)
		m_camera->translate(glm::vec3(-0.2f, 0.f, 0.f));
	else if (key == Qt::Key_D)
		m_camera->translate(glm::vec3(0.2f, 0.f, 0.f));

	update();
}