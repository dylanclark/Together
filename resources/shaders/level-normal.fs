#version 330 core

out vec4 color;

in vec2 tex_coord;
in vec2 frag_pos;

struct light {
    vec2 position;
    float range;
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
    int ring_size = 10;

    float dist = length(vec3(frag_pos, 0.0) - vec3(cur_light.position, 0.0));
    vec3 normal = texture(m_normalmap, tex_coord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = vec3(float(dir == 0) * normal.b - float(dir == 1) * normal.b, float(flipped == 1) * normal.g - float(flipped == 0) * normal.g, normal.r);
    vec3 light_dir = normalize(vec3(cur_light.position, 0.0) - vec3(frag_pos, 0.0));

    float range = floor(cur_light.range / ring_size) * ring_size;
    float attenuation = max((cur_light.range - dist) / (cur_light.range*2), 0.0);
    bool ring = false;
    if (attenuation*ring_size > 4) {
        attenuation = ceil(attenuation*ring_size) / ring_size;
        ring = true;
    }

    float norm_attenuation = max((cur_light.range - dist/2) / (cur_light.range*3), 0.0);

    float dot_prod = dot(normal, light_dir);
    float diff = max(dot_prod, 0.0);

    vec3 texcolor = vec3(texture(m_texture, tex_coord));
    vec3 diffuse = diff * norm_attenuation * obj_color * texcolor;
    if (ring && texcolor.x + texcolor.y + texcolor.z < 0.5) {
        diffuse += attenuation * obj_color * (vec3(texture(m_texture, tex_coord)) + vec3(0.18, 0.18, 0.18));
    } else {
        diffuse += attenuation * obj_color * vec3(texture(m_texture, tex_coord));
    }
    return diffuse * cur_light.strength;
}

void main()
{
    vec4 texcolor = texture(m_texture, tex_coord) * vec4(obj_color, 1.0);
    if (texcolor.a < 0.1)
        discard;

    float ambient_strength = 0.15;
    vec3 result = (ambient_strength) * (texcolor.xyz);
    for (int i = 0; i < num_lights; i++) {
        result += calculate_light(lights[i]);
    }

    color = vec4(result, 1.0);
}
