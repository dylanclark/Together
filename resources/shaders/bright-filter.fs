#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D color_texture;

void main()
{
    vec4 color = texture(color_texture, tex_coords);
    float brightness = (color.r * 0.2126) + (color.g + 0.7152) + (color.b * 0.0722);
    if (brightness > 0.5) {
        out_color = color;
    } else {
        out_color = vec4(vec3(0.0), 1.0);
    }
}
