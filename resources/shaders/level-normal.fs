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

uniform int dir;
uniform int flipped;

uniform sampler2D m_texture;
uniform sampler2D m_normalmap;
uniform vec3 obj_color;

// eventually this will accept a normal argument
vec3 calculate_light(light cur_light)
{
    int ring_size = 20;

    float dist = length(vec3(frag_pos, 0.0) - vec3(cur_light.position, 0.0));
    vec3 normal = texture(m_normalmap, tex_coord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = vec3(float(dir == 0) * normal.b - float(dir == 1) * normal.b, float(flipped == 1) * normal.g - float(flipped == 0) * normal.g, normal.r);
    vec3 light_dir = normalize(vec3(cur_light.position, 0.0) - vec3(frag_pos, 0.0));

    float strength = floor(cur_light.strength / ring_size) * ring_size;
    float attenuation = max((cur_light.strength - dist) / (cur_light.strength*5), 0.0);
    attenuation = floor(attenuation*ring_size) / ring_size;

    float dot_prod = dot(normal, light_dir);
    dot_prod = floor(dot_prod*ring_size) / ring_size;
    float diff = max(dot_prod, 0.0);

    vec3 diffuse = diff * attenuation*2 * obj_color * vec3(texture(m_texture, tex_coord));
    diffuse += attenuation * obj_color * vec3(texture(m_texture, tex_coord));
    return diffuse;
}

void main()
{
    vec4 texcolor = texture(m_texture, tex_coord) * vec4(obj_color, 1.0);
    if (texcolor.a < 0.1)
        discard;

    float ambient_strength = 0.1;
    vec3 result = (ambient_strength) * (texcolor.xyz);
    for (int i = 0; i < num_lights; i++) {
        result += calculate_light(lights[i]);
    }

    color = vec4(result, 1.0);
}
