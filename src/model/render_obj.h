#pragma once

#include "cache.h"

#include <GL/glew.h>
#include <iostream>

using SetFloatDataFunc = void (*)(GLuint, Cachef&);
using SetUnsignedDataFunc = void (*)(GLuint, Cacheu&);

class Mesh;
class ShaderProgram;

class RenderObj
{
public:
    RenderObj();
    RenderObj(int v_c_size, int n_c_size, int c_c_size, int i_c_size);
    ~RenderObj();
    RenderObj(const RenderObj& b) = delete;
    RenderObj& operator=(const RenderObj& b) = delete;
    RenderObj(RenderObj&& b);
    RenderObj& operator=(RenderObj&& b);

    inline void setSmooth(bool s) { smooth = s; }
    inline void setShaderProgram(ShaderProgram* s) { shader = s; }
    void updateData(Mesh* mesh);
    void render();

private:
    inline void setVertData(Cachef& v) {
        setVertProxy(vert_buf, v);
        std::cout << "vert size " << v.size << std::endl;
    }
    inline void setNormData(Cachef& n) {
        setNormProxy(norm_buf, n);
        std::cout << "norm size " << n.size << std::endl;
    }
    inline void setColData(Cachef& c) {
        setColProxy(col_buf, c);
        std::cout << "color size " << c.size << std::endl;
    }
    inline void setIdxData(Cacheu& i) {
        setIdxProxy(idx_buf, i);
        idx_cnt = i.size / sizeof(GLuint);
        std::cout << "idx count is " << idx_cnt << std::endl;
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
    ShaderProgram *shader;

    bool smooth;
    union
    {
        int comp_size[4];
        struct
        {
            int  vert_comp_size;
            int  idx_comp_size;
            int  norm_comp_size;
            int  col_comp_size;
        };
    };
    int idx_cnt;

    char flag;

    SetFloatDataFunc setVertProxy;
    SetFloatDataFunc setNormProxy;
    SetFloatDataFunc setColProxy;
    SetUnsignedDataFunc setIdxProxy;
};
