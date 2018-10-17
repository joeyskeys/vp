#version 330

layout(location=0) in vec3 vert;
layout(location=1) in vec3 norm;
out vec3 geom_vert;
out vec3 geom_norm;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    geom_vert = vert;
    geom_norm = normalize(norm);
    gl_Position = proj * view * vec4(vert, 1.0);
}