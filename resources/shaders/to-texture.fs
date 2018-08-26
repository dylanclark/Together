#version 330 core

in vec2 tex_coord;
out vec4 out_color;
uniform sampler2D m_texture;

void main()
{
    vec4 color = texture(m_texture, tex_coord);
    if (color.a < 0.1) {
        out_color = vec4(1.0, 1.0, 1.0, 0.0);
    } else {
        out_color = color;
    }
}
