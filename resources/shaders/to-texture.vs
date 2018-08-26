#version 330 core

layout (location = 0) in vec4 position;

out vec2 tex_coord;

uniform mat4 tex_clip;
uniform mat4 model;
uniform mat4 proj;

vec4 tex_coord_vec;

void main()
{
    gl_Position = proj * model * vec4(position.xy, 0.0, 1.0);
    tex_coord_vec = tex_clip * vec4(position.zw, 0.0, 1.0);
    tex_coord = vec2(tex_coord_vec.x, tex_coord_vec.y);
}
