#include "cache.h"

#include <GL/glew.h>

class RenderObj
{
public:
    RenderObj();
    ~RenderObj();
    RenderObj(const RenderObj& b) = delete;
    RenderObj& operator=(const RenderObj& b) = delete;
    RenderObj(RenderObj&& b);
    RenderObj& operator=(RenderObj&& b);

    inline void toggleSmooth() { smooth ~= smooth; }
    void setVertData(Cachef& v);
    void setNormData(Cachef& n);
    void setColData(Cachef& c);
    void setIdxData(Cacheu& i);

    void render();

private:
    union
    {
        GLuint vbo_list[4];
        struct
        {
            GLuint vert_buf;
            GLuint norm_buf;
            GLuint col_buf;
            GLuint idx_buf;
        };
    };

    bool smooth;
}
