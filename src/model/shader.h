#include "cache.h"
#include "manager.h"
#include "uniformtable.h"

#include <GL/glew.h>

#include <string.h>

using namespace std;

typedef struct ShaderProgramObj
{
    Str path;
    Str name;
} ShaderProgramObj;

class ShaderProgram
{
public:
    ShaderProgram();
	~ShaderProgram();
    ShaderProgram(const ShaderProgram& rhs) = delete;
    ShaderProgram& operator=(const ShaderProgram& rhs) = delete;
	ShaderProgram(ShaderProgram&& rhs);
	ShaderProgram& operator=(ShaderProgram&& rhs);

    inline bool isInitailized() const { return !program; }
    bool		load(const string& p, const string& n);
    inline void use() const { glUseProgram(program); }

    inline GLuint   getProgram() const { return program; }
    inline GLint    getUniformLocation(const char* var) const
    {
        return glGetUniformLocation(program, var);
    }

private:
    ShaderProgramObj    *s;
    GLuint              program;
    UniformTable        uniform_table;
    
    static ShaderProgramManager *mgr;
};
