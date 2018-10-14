#include "render_obj.h"
#include <model/mesh.h>
#include <model/table.h>
#include <model/shader.h>

#include <cstring>
#include <iostream>

#include <QImage>
#include <QString>

const static char NORMAL =   1;
const static char COLOR =    2;

static void setFloatData(GLuint buf, Cachef& c)
{
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, c.size, c.data, GL_STATIC_DRAW);
}

static void setFloatDummy(GLuint buf, Cachef& c)
{
    return;
}

static void SetUnsignedData(GLuint buf, Cacheu& c)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, c.size, c.data, GL_STATIC_DRAW);
}

static void setUnsignedDummy(GLuint buf, Cacheu& c)
{
    return;
}

RenderObj::RenderObj():
    vao(0),
    vert_buf(0),
    idx_buf(0),
    norm_buf(0),
    col_buf(0),
    fbo(0),
    rbo_color0(0),
    rbo_color1(1),
    rbo_depth(0),
    shader(nullptr),
    global_uniforms(nullptr),
    smooth(true),
    vert_comp_size(3),
    idx_comp_size(3),
    norm_comp_size(3),
    col_comp_size(4),
    idx_cnt(0),
    flag(NORMAL & COLOR),
    setVertProxy(&setFloatData),
    setNormProxy(&setFloatData),
    setColProxy(&setFloatData),
    setIdxProxy(&SetUnsignedData)
{
    glGenBuffers(4, vbo_list);
};

RenderObj::RenderObj(int v_c_size, int n_c_size, int c_c_size, int i_c_size):
    vao(0),
    vert_buf(0),
    idx_buf(0),
    norm_buf(0),
    col_buf(0),
    fbo(0),
    rbo_color0(0),
    rbo_color1(0),
    rbo_depth(0),
    shader(nullptr),
    global_uniforms(nullptr),
    smooth(true),
    vert_comp_size(v_c_size),
    idx_comp_size(i_c_size),
    norm_comp_size(n_c_size),
    col_comp_size(c_c_size),
    idx_cnt(0),
    flag((n_c_size ? NORMAL : 0) & (c_c_size ? COLOR : 0)),
    setVertProxy(&setFloatData),
    setNormProxy(n_c_size ? &setFloatData : &setFloatDummy),
    setColProxy(c_c_size ? &setFloatData : &setFloatDummy),
    setIdxProxy(&SetUnsignedData)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo_list);
    if (n_c_size)
        glGenBuffers(1, vbo_list + 2);
    if (c_c_size)
        glGenBuffers(1, vbo_list + 3);
}

RenderObj::~RenderObj()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(4, vbo_list);
    if (fbo)
        clearFBO();
}

RenderObj::RenderObj(RenderObj&& b)
{
    memcpy(vbo_list, b.vbo_list, 4 * sizeof(GLuint));
    memset(b.vbo_list, 0, 4 * sizeof(GLuint));
}

RenderObj& RenderObj::operator=(RenderObj&& b)
{
    memcpy(vbo_list, b.vbo_list, 4 * sizeof(GLuint));
    return *this;
}

void RenderObj::updateData(Mesh* mesh)
{
    glBindVertexArray(vao);
    if (smooth)
    {
        setVertData(mesh->getVertCache());
        setNormData(mesh->getNormCache());
        setColData(mesh->getColCache());
        setIdxData(mesh->getIdxCache());
    }
    else
    {
        setVertData(mesh->getExpandedVertCache());
        setNormData(mesh->getExpandedNormCache());
    }

    for (AttribTable::iterator it = shader->attrib_table.begin(); it != shader->attrib_table.end(); ++it)
    {
        glEnableVertexAttribArray(it->first);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_list[it->second]);
        glVertexAttribPointer(it->first, comp_size[it->second], GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    glBindVertexArray(0);
}

void RenderObj::updateData(Cachef* vcache, Cachef* ncache, Cachef* ccache, Cacheu* icache)
{
    if (vcache)
        setVertData(*vcache);
    if (ncache)
        setNormData(*ncache);
    if (ccache)
        setColData(*ccache);
    if (icache)
        setIdxData(*icache);
}

void RenderObj::render()
{
    shader->use();
    global_uniforms->updateLocation(shader);
    global_uniforms->uploadUniforms();
    shader->uniform_table.uploadUniforms();

    glBindVertexArray(vao);
    if (smooth)
        glDrawElements(GL_TRIANGLES, idx_cnt, GL_UNSIGNED_INT, (void*)0);
    else
        glDrawArrays(GL_TRIANGLES, 0, idx_cnt);
    glBindVertexArray(0);
}

void RenderObj::initFBO(int w, int h)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(1, &rbo_color0);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_color0);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, w, h);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB32F, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_color0);

    // GL_RED_INTEGER is not a valid iternal format for glRenderbufferStorage
    glGenRenderbuffers(1, &rbo_color1);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_color1);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_R32I, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, rbo_color1);

    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "error : framebuffer is not complete" << std::endl;
    else
        std::cout << "framebuffer is complete" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderObj::clearFBO()
{
    glDeleteRenderbuffers(1, &rbo_color0);
    glDeleteRenderbuffers(1, &rbo_color1);
    glDeleteRenderbuffers(1, &rbo_depth);
    glDeleteFramebuffers(1, &fbo);
    rbo_color0 = rbo_color1 = rbo_depth = fbo = 0;
}

void RenderObj::renderToFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    const GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, buffers);
    if (smooth)
        glDrawElements(GL_TRIANGLES, idx_cnt, GL_UNSIGNED_INT, (void*)0);
    else
        glDrawArrays(GL_TRIANGLES, 0, idx_cnt);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderObj::readPixelFromFBO(int x, int y, void* data)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderObj::renderAndReadFromFBO(int x, int y, void* data)
{
    shader->use();
    global_uniforms->updateLocation(shader);
    global_uniforms->uploadUniforms();
    shader->uniform_table.uploadUniforms();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    int default_idx = -1;
    glClearColor(0.f, 0.f, 0.f, *(float*)(&default_idx));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    const GLuint buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, buffers);
    glBindVertexArray(vao);
    if (smooth)
        glDrawElements(GL_TRIANGLES, idx_cnt, GL_UNSIGNED_INT, (void*)0);
    else
        glDrawArrays(GL_TRIANGLES, 0, idx_cnt);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    //glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, data);
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, data);
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, (int*)(data) + 3);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

