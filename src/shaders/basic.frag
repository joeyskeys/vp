#version 330

in vec3 frag_vert;
in vec3 frag_norm;
out vec4 out_color;

uniform mat4 view;

uniform struct PointLight
{
	vec3 position;
	vec3 color;	
} light;

void main()
{
    mat3 norm_mat = transpose(inverse(mat3(view)));
    vec3 norm = normalize(norm_mat * frag_norm);
    vec3 pos = vec3(view * vec4(frag_vert, 1));
    vec3 frag_to_light = light.position - pos;
    float brightness = dot(norm, frag_to_light) / (length(norm) * length(frag_to_light));

    //out_color = vec4(light.color, 1.) * brightness;
    out_color = vec4(1.0, 0.0, 0.0, 1.0) * brightness;
}
