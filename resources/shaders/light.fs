#version 330 core

out vec4 color;

in vec2 tex_coord;
in vec2 frag_pos;

uniform sampler2D m_texture;
uniform vec3 obj_color;

void main()
{
    vec4 out_color = texture(m_texture, tex_coord) * vec4(obj_color*1.3, 1.0);
    if (out_color.a < 0.1)
        discard;

    color = out_color;
}
