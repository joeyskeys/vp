#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
in vec3 geom_vert[];
in vec3 geom_norm[];
out vec3 frag_vert;
out vec3 frag_norm;
out vec3 barycentric;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    barycentric = vec3(1.0, 0.0, 0.0);
    frag_vert = geom_vert[0];
    frag_norm = geom_norm[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    barycentric = vec3(0.0, 1.0, 0.0);
    frag_vert = geom_vert[1];
    frag_norm = geom_norm[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    barycentric = vec3(0.0, 0.0, 1.0);
    frag_vert = geom_vert[2];
    frag_norm = geom_norm[2];
    EmitVertex();
}