#version 330

layout(location=0) in vec3 vert;
layout(location=1) in vec3 norm;
out vec3 frag_vert;
out vec3 frag_norm;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    frag_vert = vert;
    //frag_norm = (view * vec4(norm, 0.)).xyz;
    frag_norm = norm;
    gl_Position = proj * view * vec4(vert, 1.0);
}