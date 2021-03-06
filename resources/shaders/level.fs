#version 330 core

out vec4 color;

in vec2 tex_coord;
in vec2 frag_pos;

struct light {
    vec2 position;
    float strength;
};

#define MAX_LIGHTS 64
uniform light lights[MAX_LIGHTS];
uniform int num_lights;

uniform sampler2D m_texture;
uniform sampler2D m_normalmap;
uniform vec3 obj_color;

// eventually this will accept a normal argument
vec3 calculate_light(light cur_light, vec2 cur_frag_pos)
{
    int ring_size = 20;
    float dist = length(vec3(cur_frag_pos, 0.0) - vec3(cur_light.position, 0.0));

    float strength = floor(cur_light.strength / ring_size) * ring_size;
    float attenuation = max((cur_light.strength - dist) / (cur_light.strength*5), 0.0);
    attenuation = floor(attenuation*ring_size) / ring_size;

    vec3 diffuse = obj_color * vec3(texture(m_texture, tex_coord));
    return attenuation * diffuse;
}

void main()
{
    vec4 texcolor = texture(m_texture, tex_coord) * vec4(obj_color, 1.0);
    if (texcolor.a < 0.1)
        discard;

    float ambient_strength = 0.1;
    vec3 result = (ambient_strength) * (texcolor.xyz);
    for (int i = 0; i < num_lights; i++) {
        result += calculate_light(lights[i], frag_pos);
    }

    color = vec4(result, 1.0);
}
