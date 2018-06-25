#include "viewport.h"
#include <QOpenGLShaderProgram>

#include <cstring>

QtViewport::QtViewport(QWidget *parent) :
    QOpenGLWidget(parent),
    m_prog(0)
{
    m_mesh.fillTestData();
	m_light.setType(LIGHT_PNT);
	m_light.setPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	m_light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

QtViewport::~QtViewport()
{
    makeCurrent();

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

	m_prog = glCreateProgram();
	GLuint vshader, fshader;
	
	vshader = glCreateShader(GL_VERTEX_SHADER);
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	int len = strlen(vsrc);
	glShaderSource(vshader, 1, &vsrc, &len);
	glCompileShader(vshader);
	len = strlen(fsrc);
	glShaderSource(fshader, 1, &fsrc, &len);
	glCompileShader(fshader);
	glAttachShader(m_prog, vshader);
	glAttachShader(m_prog, fshader);
	glLinkProgram(m_prog);

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
	if (m_prog)
		glDeleteProgram(m_prog);
}

void QtViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_prog);

	m_proj_val = glm::perspective(1.047f, 4.f / 3.f, 1.f, 100.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(m_proj_val));
	m_view_val = glm::mat4(1.f);
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(m_view_val));
	glUniform3fv(m_light_position_loc, 1, (GLfloat*)m_light.getData());

    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo1);
    glVertexAttribPointer(0, m_mesh.getVertCount(), GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glVertexAttribPointer(1, m_mesh.getNormCount(), GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
	
}

void QtViewport::resizeGL(int width, int height)
{
    return;
}
