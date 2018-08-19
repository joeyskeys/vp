#include "render_obj.h"

#include <cstring>

RenderObj::RenderObj():
    smooth(true)
{
    glGenBuffers(4, vbo_list);
};

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
    *this(b);
}

void RenderObj::setVertData(Cachef& v)
{
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    glBufferData(GL_ARRAY_BUFFER, v.size, v.data, GL_STATIC_DRAW);
}

void RenderObj::setNormData(Cachef& n)
{
    glBindBuffer(GL_ARRAY_BUFFER, norm_buf);
    glBufferData(GL_ARRAY_BUFFER, n.size, n.data, GL_STATIC_DRAW);
}

void RenderObj::setColData(Cachef& c)
{
    glBindBuffer(GL_ARRAY_BUFFER, col_buf);
    glBufferData(GL_ARRAY_BUFFER, c.size, n.data, GL_STATIC_DRAW);
}

void RenderObj::setIdxData(Cacheu& i)
{
    glBindBuffer(GL_ARRAY_BUFFER, idx_buf);
    glBufferData(GL_ARRAY_BUFFER, i.size, i.data, GL_STATIC_DRAW);
}

void RenderObj::render()
{
    // todo : put shader related stuff here
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, norm_buf);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    if (smooth)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
        // todo : need to add member to store indices array size
        //glDrawElements(GL_TRIANGLES, idx_size, GL_UNSIGNED_INT, (void*)0);
    }
    else
    {
        //glDrawArrays(GL_TRIANGLES, 0, idx_cnt);
    }
}
