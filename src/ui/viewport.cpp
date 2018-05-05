#include "viewport.h"
#include <QOpenGLShaderProgram>

QtViewport::QtViewport(QWidget *parent) :
    QOpenGLWidget(parent),
    m_program(0)
{

}

QtViewport::~QtViewport()
{
    makeCurrent();

    if (m_program)
        delete m_program;
    m_vbo.destroy();

    doneCurrent();
}

void QtViewport::initializeGL()
{
    initializeOpenGLFunctions();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    const char *vsrc = 
        "#version 330\n"
        "layout(location=0) in vec3 vert;\n"
        "uniform mat4 proj;\n"
        "uniform mat4 view;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = proj * view * vert;\n"
        "}";

    const char *fsrc =
        "#version 330\n"
        "out vec4 outc;\n"
        "void main()\n"
        "{\n"
        "   outc = vec4(1., 0., 0., 1.);\n"
        "}";

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    m_program->link();

    m_proj_loc = m_program->uniformLocation("proj");
    m_view_loc = m_program->uniformLocation("view");
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

    static GLfloat verts[] = {
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.f, 0.5f, 0.f
    };

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, verts);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void QtViewport::resizeGL(int width, int height)
{
    return;
}
