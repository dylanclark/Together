#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D m_texture;
uniform vec3 obj_color;

void main()
{
    vec4 color = texture(m_texture, tex_coords) * vec4(obj_color, 1.0);
}
