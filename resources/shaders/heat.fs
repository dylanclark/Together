#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform float time;
uniform sampler2D color_texture;

float ampl = 1.5;
float speed = 2.0;
float wavelength = 50.0;

void main()
{
    vec2 new_tex_coords = vec2(tex_coords.x + sin(tex_coords.y*wavelength+time*speed)/(1000/ampl), tex_coords.y);
    out_color = texture(color_texture, new_tex_coords);
}
