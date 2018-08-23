#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D color_texture;

uniform float timer;
uniform float screen_x;
uniform float screen_y;

const vec3 shock_params = vec3 ( 10.0, 0.8, 0.1 );
const float vignette_size = 0.3;
const float tolerance = 0.01;

void main()
{
    vec2 center = vec2(screen_x,screen_y);
    float dist = distance(tex_coords, center);
    vec2 new_tex_coords = tex_coords;

    if ((dist <= (timer + shock_params.z)) &&
         (dist >= (timer - shock_params.z)))
    {
        float diff = (dist - timer);
        float pow_diff = 1.0 - pow(abs(diff*shock_params.x), shock_params.y);
        float diff_time = diff * pow_diff;
        vec2 diff_uv = normalize(tex_coords - center);
        new_tex_coords = tex_coords + (diff_uv * diff_time);
    }

    vec2 delta_tex_coords = vec2(screen_x,screen_y) - new_tex_coords;
    vec2 powers = vec2(pow(delta_tex_coords.x, 2.0), pow(delta_tex_coords.y, 2.0));

	float radius_sqrd = pow(vignette_size, 2.0);
	float gradient = smoothstep(radius_sqrd-tolerance, radius_sqrd+tolerance, powers.x+powers.y*0.3);

	out_color = mix(texture(color_texture, new_tex_coords), texture(color_texture, tex_coords), gradient);
    out_color.a = 1.0;
}
