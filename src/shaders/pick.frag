#version 330

in vec3 frag_vert;
//out vec4 out_color;
layout(location=0) out vec3 out_color;
layout(location=1) out int  out_index;

void main()
{
    //out_color = vec4(frag_vert, gl_PrimitiveID);
    out_color = vec3(frag_vert);
    out_index = gl_PrimitiveID;
}
