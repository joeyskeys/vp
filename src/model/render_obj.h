#pragma once

#include "cache.h"

#include <GL/glew.h>

using SetFloatDataFunc = void (*)(GLuint, Cachef&);
using SetUnsignedDataFunc = void (*)(GLuint, Cacheu&);

class Mesh;
class Shader;

class RenderObj
{
public:
    RenderObj();
    RenderObj(int v_c_size, int n_c_size, int c_c_size);
    ~RenderObj();
    RenderObj(const RenderObj& b) = delete;
    RenderObj& operator=(const RenderObj& b) = delete;
    RenderObj(RenderObj&& b);
    RenderObj& operator=(RenderObj&& b);

    inline void setSmooth(bool s) { smooth = s; }
    inline void setShader(Shader* s) { shader = s; }
    void updateData(Mesh* mesh);
    void render();

private:
    inline void setVertData(Cachef& v) {
        setVertProxy(vert_buf, v);
    }
    inline void setNormData(Cachef& n) {
        setNormProxy(norm_buf, n);
    }
    inline void setColData(Cachef& c) {
        setColProxy(col_buf, c);
    }
    inline void setIdxData(Cacheu& i) {
        setIdxProxy(idx_buf, i);
        idx_cnt = i.size / sizeof(GLuint);
    }
    

private:
    union
    {
        GLuint vbo_list[4];
        struct
        {
            GLuint vert_buf;
            GLuint idx_buf;
            GLuint norm_buf;
            GLuint col_buf;
        };
    };

    //Mesh *mesh;
    Shader *shader;

    bool smooth;
    int  vert_comp_size;
    int  norm_comp_size;
    int  col_comp_size;
    int  idx_cnt;

    char flag;

    SetFloatDataFunc setVertProxy;
    SetFloatDataFunc setNormProxy;
    SetFloatDataFunc setColProxy;
    SetUnsignedDataFunc setIdxProxy;
};
