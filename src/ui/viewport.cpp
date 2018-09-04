#include "viewport.h"

#include <io/obj_loader.h>
#include <model/table.h>

#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QCursor>
#include <QPoint>

#include <cstring>
#include <stdexcept>
#include <cstdlib>

QtViewport::QtViewport(QWidget *parent) :
	QOpenGLWidget(parent),
	m_prog(0),
	m_move_enabled(false),
	m_rotate_enabled(false)
{
    //m_mesh.fillTriangle();
    m_mesh.fillCube();
    //m_mesh.updateExpandedMesh();
    //ObjLoader loader;
    //loader.load("/mnt/media/workspace/repos/self/vp/src/asset/cube.obj");
    //loader.fillMesh(&m_mesh);
    std::cout << "load finished" << std::endl;
    std::cout << "vert cnt : " << m_mesh.getVertCount() << std::endl;
    std::cout << "idx cnt : " << m_mesh.getTriCount() << std::endl;
	m_light.setType(LIGHT_PNT);
	m_light.setPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	m_light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_program = new ShaderProgram;
    m_global_uniforms = new UniformTable();
	m_camera = new Camera;
	QPoint p = mapFromGlobal(QCursor::pos());
	m_mouse_pos = glm::vec2(p.x(), p.y());
}

QtViewport::~QtViewport()
{
    makeCurrent();

    delete m_program;
    delete m_global_uniforms;
	delete m_camera;

	clearGL();

    doneCurrent();
}

void QtViewport::initializeGL()
{
    std::cout << "init glew" << std::endl;
	if (glewInit() != GLEW_OK)
    {
        std::logic_error ex("Failed to initialize GLEW\n");
        std::cout << "glew init failed" << std::endl;
		throw std::exception(ex);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	//m_program->load("E:/work/repos/vp/src/shaders/", "basic");
    //m_global_uniforms->loadDescription("/mnt/media/workspace/repos/self/vp/src/shaders/global_uniforms.json");
    m_global_uniforms->loadDescription("/home/chenmiwei/Work/source/vp/src/shaders/global_uniforms.json");
    //m_program->load("/home/joey/Desktop/workspace/repos/self/vp/src/shaders/", "basic");
    if (!m_program->load("/home/chenmiwei/Work/source/vp/src/shaders/", "basic"))
    {
        std::cout << "shader init failed" << std::endl;
        exit(1);
    }
	m_prog = m_program->getProgram();
    m_renderobj = new RenderObj(3, 3, 0, 3);
    m_renderobj->setShaderProgram(m_program);
    m_renderobj->updateData(&m_mesh);

	m_proj_loc = glGetUniformLocation(m_prog, "proj");
	m_view_loc = glGetUniformLocation(m_prog, "view");
	m_light_position_loc = glGetUniformLocation(m_prog, "light.position");
	m_light_color_loc = glGetUniformLocation(m_prog, "light.color");
    
    m_global_uniforms->updateLocation(m_program);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo1);
	glBufferData(GL_ARRAY_BUFFER, m_mesh.getVertSize(), m_mesh.getVerts(), GL_STATIC_DRAW);
	glGenBuffers(1, &m_vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glBufferData(GL_ARRAY_BUFFER, m_mesh.getNormSize(), m_mesh.getNorms(), GL_STATIC_DRAW);
    glGenBuffers(1, &m_vbo_idx);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_idx);
    glBufferData(GL_ARRAY_BUFFER, m_mesh.getIdxSize(), m_mesh.getIdx(), GL_STATIC_DRAW);
    std::cout << "old size\nvert " << m_mesh.getVertSize() << "\nnorm " << m_mesh.getNormSize() << std::endl;
}

void QtViewport::clearGL()
{
	glDeleteBuffers(1, &m_vbo1);
	glDeleteBuffers(1, &m_vbo2);
    glDeleteBuffers(1, &m_vbo_idx);
	glDeleteBuffers(1, &m_vao);
	//if (m_prog)
	//	glDeleteProgram(m_prog);
}

void QtViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUseProgram(m_prog);
	m_program->use();

	m_proj_val = glm::perspective(1.047f, 4.f / 3.f, 1.f, 100.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(m_proj_val));

	//glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_camera->getViewMatrixPtr());
	//glUniform3fv(m_light_position_loc, 1, (GLfloat*)m_light.getData());

	m_global_uniforms->updateUniform("proj", glm::value_ptr(m_proj_val));
    m_global_uniforms->updateUniform("view", m_camera->getViewMatrixPtr());
    m_global_uniforms->uploadUniforms();

    /*
    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //glDrawArrays(GL_TRIANGLES, 0, m_mesh.getIdxCount());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_idx);
    glDrawElements(GL_TRIANGLES, m_mesh.getIdxSize(), GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    std::cout << "idx cnt " << m_mesh.getIdxCount() << std::endl;
    */
    
    m_renderobj->render();
}

void QtViewport::resizeGL(int width, int height)
{
    return;
}

void QtViewport::keyPressEvent(QKeyEvent *event)
{
	int key = event->key();

	if (key == Qt::Key_W)
		//m_camera->translate(glm::vec3(0.f, 0.f, -0.2f));
		m_camera->translateAroundPivot(0.f, 0.05f);
	else if (key == Qt::Key_S)
		//m_camera->translate(glm::vec3(0.f, 0.f, 0.2f));
		m_camera->translateAroundPivot(0.f, -0.05f);
	else if (key == Qt::Key_A)
		//m_camera->translate(glm::vec3(-0.2f, 0.f, 0.f));
		m_camera->translateAroundPivot(-0.05f, 0.f);
	else if (key == Qt::Key_D)
		//m_camera->translate(glm::vec3(0.2f, 0.f, 0.f));
		m_camera->translateAroundPivot(0.05f, 0.f);

	update();
}

void QtViewport::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::MidButton)
	{
		if (event->modifiers() & Qt::SHIFT)
			m_move_enabled = true;
		else
			m_rotate_enabled = true;
	}

	m_mouse_pos = glm::vec2(event->x(), event->y());
}

void QtViewport::mouseReleaseEvent(QMouseEvent *event)
{
	m_rotate_enabled = false;
	m_move_enabled = false;
}

void QtViewport::mouseMoveEvent(QMouseEvent *event)
{
	glm::vec2 cur_pos(event->x(), event->y());
	glm::vec2 delta = cur_pos - m_mouse_pos;

	if (m_rotate_enabled)
		m_camera->rotateAroundPivot(delta.x, delta.y);
	if (m_move_enabled)
		m_camera->translateAroundPivot(delta.x / width(), delta.y / height());

	m_mouse_pos = cur_pos;

	update();
}

void QtViewport::wheelEvent(QWheelEvent *event)
{
	m_camera->zoom(-event->delta() / 360.f);

	update();
}
