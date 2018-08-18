#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 raw_tex_coord;

out vec2 tex_coord;

uniform mat2 tex_clip;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
    tex_coord = tex_clip * raw_tex_coord;
}
