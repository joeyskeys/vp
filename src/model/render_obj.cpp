#include "render_obj.h"
#include <model/mesh.h>
#include <model/table.h>
#include <model/shader.h>

#include <cstring>
#include <iostream>

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
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, c.size, c.data, GL_STATIC_DRAW);
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
    rbo_color(0),
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
{jG
    glGenBuffers(4, vbo_list);
};

RenderObj::RenderObj(int v_c_size, int n_c_size, int c_c_size, int i_c_size):
    vao(0),
    vert_buf(0),
    idx_buf(0),
    norm_buf(0),
    col_buf(0),
    fbo(0),
    rbo_color(0),
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
        setNormData(mesh->getColCache());
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

    if (smooth)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
    else
        glDrawArrays(GL_TRIANGLES, 0, idx_cnt);

    for (AttribTable::iterator it = shader->attrib_table.begin(); it != shader->attrib_table.end(); ++it)
        glDisableVertexAttribArray(it->first);

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
    global_uniforms->uploadUniforms();
    shader->uniform_table.uploadUniforms();

    /*
    for (AttribTable::iterator it = shader->attrib_table.begin(); it != shader->attrib_table.end(); ++it)
    {
        glEnableVertexAttribArray(it->first);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_list[it->second]);
        glVertexAttribPointer(it->first, comp_size[it->second], GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    if (smooth)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
        glDrawElements(GL_TRIANGLES, idx_cnt, GL_UNSIGNED_INT, (void*)0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, idx_cnt);
    }

    for (AttribTable::iterator it = shader->attrib_table.begin(); it != shader->attrib_table.end(); ++it)
        glDisableVertexAttribArray(it->first);
    */

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
    glGenRenderbuffers(1, &rbo_color);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_color);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_color);
    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
    if (glCheckFrameBufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "error : framebuffer is not complete" << std::endl;
    glBinderFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderObj::clearFBO()
{
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}

void RenderObj::renderToFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    if (smooth)
        glDrawElements(GL_TRIANGLES, idx_cnt, GL_UNSIGNED_INT, (void*)0);
    else
        glDrawArrays(GL_TRIANGLES, 0, idx_cnt);
    glBindVertexArray(0);
}

void readPixelFromFBO()
{

}