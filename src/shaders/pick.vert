#version 330

layout(location=0) in vec3 vert;
out vec3 frag_vert;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    frag_vert = vert;
    gl_Position = proj * view * vec4(vert, 1.0);
}
