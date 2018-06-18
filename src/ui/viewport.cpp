#include "viewport.h"
#include <QOpenGLShaderProgram>

QtViewport::QtViewport(QWidget *parent) :
    QOpenGLWidget(parent),
    m_program(0)
{
    m_mesh.fillTestData();
	m_light.setType(LIGHT_PNT);
	m_light.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	m_light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

QtViewport::~QtViewport()
{
    makeCurrent();

    if (m_program)
        delete m_program;

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

	const char *vsrc =
		"#version 330\n"
		"layout(location=0) in vec3 vert;\n"
		"layout(location=1) in vec3 norm;\n"
		"out vec3 frag_vert;\n"
		"out vec3 frag_norm;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 view;\n"
		"void main()\n"
		"{\n"
		"   frag_vert = vert;\n"
		"   frag_norm = norm;\n"
        "   gl_Position = proj * view * vec4(vert, 1.0);\n"
        "}";

	const char *fsrc =
		"#version 330\n"
		"in vec3 frag_vert;\n"
		"in vec3 frag_norm;\n"
		"out vec4 outc;\n"
		"uniform mat4 view;\n"
		"uniform struct PointLight\n"
		"{\n"
		"   vec3 position;\n"
		"   vec3 color;\n"
		"} light;\n"
		"void main()\n"
		"{\n"
		"   mat3 norm_mat = transpose(inverse(mat3(view)));\n"
		"   vec3 norm = normalize(norm_mat * frag_norm);\n"
		"   vec3 pos = vec3(view * vec4(frag_vert, 1));\n"
		"   vec3 frag_to_light = light.position - pos;\n"
		"   float brightness = dot(norm, frag_to_light) / (length(norm) * length(frag_to_light));\n"
        "   outc = vec4(1., 0., 0., 1.) * brightness;\n"
        "}";

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    m_program->link();

    m_proj_loc = m_program->uniformLocation("proj");
    m_view_loc = m_program->uniformLocation("view");
	m_light_position_loc = m_program->uniformLocation("light.position");
	m_light_color_loc = m_program->uniformLocation("light.color");

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
}

void QtViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program->bind();
    m_proj_val.setToIdentity();
    m_proj_val.perspective(60.f, 1.2f, 1.f, 100.f);
    m_program->setUniformValue("proj", m_proj_val);
    m_view_val.setToIdentity();
    m_program->setUniformValue("view", m_view_val);
	glUniform3fv(m_light_position_loc, 1, (GLfloat*)m_light.getData());

    static GLfloat verts[] = {
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.f, 0.5f, 0.f
    };

    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo1);
    glVertexAttribPointer(0, m_mesh.getVertCount(), GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_mesh.getVerts());
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glVertexAttribPointer(1, m_mesh.getNormCount(), GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, m_mesh.getNorms());
    glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
	
}

void QtViewport::resizeGL(int width, int height)
{
    return;
}
