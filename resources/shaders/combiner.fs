#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 color1 = texture(texture1, tex_coords);
    vec4 color2 = texture(texture2, tex_coords);
    out_color = color1 + color2;
}
