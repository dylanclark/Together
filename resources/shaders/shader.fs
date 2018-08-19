#version 330 core

out vec4 color;

in vec2 tex_coord;

uniform sampler2D m_texture;
uniform vec3 obj_color;

void main()
{
    vec4 texcolor = texture(m_texture, tex_coord) * vec4(obj_color, 1.0);
    if (texcolor.a < 0.1)
        discard;
    color = texcolor;
}
