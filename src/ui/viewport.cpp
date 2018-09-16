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
	m_move_enabled(false),
	m_rotate_enabled(false)
{
    //m_mesh.fillTriangle();
    m_mesh.fillCube();
    //m_mesh.updateExpandedMesh();
    //ObjLoader loader;
    //loader.load("/mnt/media/workspace/repos/self/vp/src/asset/cube.obj");
    //loader.fillMesh(&m_mesh);
	m_light.setType(LIGHT_PNT);
	m_light.setPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	m_light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_program = new ShaderProgram;
    m_pick_program = new ShaderProgram;
    m_global_uniforms = new UniformTable();
	m_camera = new Camera;
	QPoint p = mapFromGlobal(QCursor::pos());
	m_mouse_pos = glm::vec2(p.x(), p.y());
    m_pixel[0] = m_pixel[1] = m_pixel[2] = m_pixel[3] = 0;
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
    m_global_uniforms->loadDescription("/mnt/media/workspace/repos/self/vp/src/shaders/global_uniforms.json");
    //m_global_uniforms->loadDescription("/home/chenmiwei/Work/source/vp/src/shaders/global_uniforms.json");
    if (!m_program->load("/home/joey/Desktop/workspace/repos/self/vp/src/shaders/", "basic"))
    //if (!m_program->load("/home/chenmiwei/Work/source/vp/src/shaders/", "pick"))
    {
        std::cout << "shader init failed" << std::endl;
        exit(1);
    }
    if (!m_pick_program->load("/mnt/media/workspace/repos/self/vp/src/shaders/", "pick"))
    {
        std::cout << "pick shader init failed" << std::endl;
        exit(1);
    }

    m_renderobj = new RenderObj(3, 3, 0, 3);
    //m_renderobj = new RenderObj(3, 0, 0, 3);
    m_renderobj->setShaderProgram(m_program);
    m_renderobj->setGlobalUniform(m_global_uniforms);
    m_renderobj->updateData(&m_mesh);

    m_pick_rdo = new RenderObj(3, 0, 0, 3);
    m_pick_rdo->setShaderProgram(m_pick_program);
    m_pick_rdo->setGlobalUniform(m_global_uniforms);
    m_pick_rdo->updateData(&m_mesh);
    m_pick_rdo->initFBO(width(), height());
}

void QtViewport::clearGL()
{

}

void QtViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_proj_val = glm::perspective(1.047f, 4.f / 3.f, 1.f, 100.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(m_proj_val));

	m_global_uniforms->updateUniform("proj", glm::value_ptr(m_proj_val));
    m_global_uniforms->updateUniform("view", m_camera->getViewMatrixPtr());
    
    //m_renderobj->render();
    m_pick_rdo->render();
    QPoint cur_pos = mapFromGlobal(QCursor::pos());
    m_pick_rdo->renderAndReadFromFBO(cur_pos.x(), cur_pos.y(), m_pixel);
    std::cout << "x y info " << cur_pos.x() << " " << cur_pos.y() << std::endl;
    std::cout << "pixel info " << m_pixel[0] << " " << m_pixel[1] << " " << m_pixel[2] << " " << m_pixel[3] << std::endl;
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
