#version 330

in(location=0) vec3 pos;
in(location=1) vec3 norm;
out vec3 frag_pos;
out vec3 frag_norm;

uniform vec3 pt_light_pos;
uniform mat4 proj;
uniform mat4 trans;

void main()
{
    frag_pos = pos;
    frag_norm = norm;
    gl_Position = proj * trans * pos;
}
