#version 330 core

out vec4 color;

in vec2 tex_coord;

uniform sampler2D m_texture;

void main()
{
    vec4 texcolor = texture(m_texture, tex_coord);
    if (texcolor.a < 0.1)
        discard;
    color = texcolor;
}
