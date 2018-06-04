#version 330

in vec3 frag_pos;
in vec3 frag_norm;
out vec4 out_color;

uniform mat4 trans;

uniform struct PointLight {
    vec3 pos;
    vec3 color;
} pt_light;

void main()
{
    mat3 norm_mat = transpose(inverse(mat3(trans)));
    vec3 norm = normalize(norm_mat * frag_norm);

    vec3 pos = vec3(trans * vec4(frag_pos, 1));

    float brightness = dot(norm, vec3(0, -1, 0)) / length(norm);

    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    out_color = color * brightness;
}
