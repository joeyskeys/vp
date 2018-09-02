#version 330

in vec3 frag_vert;
flat in float frag_index;
out vec4 out_color;

void main()
{
    out_color = vec4(frag_vert, frag_index);
}
