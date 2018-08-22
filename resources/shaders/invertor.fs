#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D color_texture;

void main()
{
    out_color = vec4(vec3(1.0 - texture(color_texture, tex_coords)), 1.0);
}
