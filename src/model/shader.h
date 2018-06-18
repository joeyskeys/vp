#include "cache.h"
#include "manager.h"

#include <GL/glew.h>

#include <string.h>

using namespace std;

typedef struct ShaderProgramObj
{
    Str path;
    Str name;
} ShaderProgramObj;

class ShadeProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    inline bool isInitailized() const { return !program; }
    bool    load(const string& p, const string& n);
    inline void use() const { glUseProgram(program); }

    inline GLuint   getProgram() const { return program; }
    inline GLint    getUniformLocation(const char* var) const
    {
        return glGetUniformLocation(program, var);
    }

private:
    ShaderProgramObj *s;
    GLuint  program;

    ShaderProgram(const ShaderProgram& rhs);
    ShaderProgram& operator=(const ShaderProgram& rhs);

    static ShaderProgramManager *mgr;
};
