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
    vert_buf(0),
    idx_buf(0),
    norm_buf(0),
    col_buf(0),
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
    vert_buf(0),
    idx_buf(0),
    norm_buf(0),
    col_buf(0),
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
    glGenBuffers(2, vbo_list);
    if (n_c_size)
        glGenBuffers(1, vbo_list + 2);
    if (c_c_size)
        glGenBuffers(1, vbo_list + 3);
}

RenderObj::~RenderObj()
{
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
}

void RenderObj::render()
{
    shader->use();
    global_uniforms->uploadUniforms();
    shader->uniform_table.uploadUniforms();

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
}
