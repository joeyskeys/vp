#version 330

layout(location=0) in vec3 vert;
layout(location=1) in float index;
out vec3 frag_vert;
flat out float frag_index;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    frag_vert = vert;
    frag_index = index;
    gl_Position = proj * view * vec4(vert, 1.0);
}
