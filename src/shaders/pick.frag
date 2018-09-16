#version 330

in vec3 frag_vert;
out vec4 out_color;

void main()
{
    out_color = vec4(frag_vert, gl_PrimitiveID);
    //out_color = vec4(gl_PrimitiveID / 10.0);
    //out_color = vec4(1.0);
}
