#version 330 core

out vec4 color;

in vec2 tex_coord;
in vec3 frag_pos;

uniform sampler2D m_texture;
uniform sampler2D m_normalmap;
uniform vec3 obj_color;
uniform vec3 light_pos;

void main()
{
    vec4 texcolor = texture(m_texture, tex_coord) * vec4(obj_color, 1.0);
    if (texcolor.a < 0.1)
        discard;

    float ambient_strength = 0.7;

    vec3 normal = texture(m_normalmap, tex_coord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diffuse = max(dot(normal, light_dir), 0.0);

    vec3 result = (ambient_strength) * (texcolor.xyz);
    color = vec4(result, 1.0);
}
