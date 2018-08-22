#version 330 core

in vec2 blur_tex_coords[11];

out vec4 out_color;

uniform sampler2D color_texture;

void main()
{
    out_color = vec4(0.0);
    out_color += texture(color_texture, blur_tex_coords[0]) * 0.0093;
    out_color += texture(color_texture, blur_tex_coords[1]) * 0.028002;
    out_color += texture(color_texture, blur_tex_coords[2]) * 0.065984;
    out_color += texture(color_texture, blur_tex_coords[3]) * 0.121703;
    out_color += texture(color_texture, blur_tex_coords[4]) * 0.175713;
    out_color += texture(color_texture, blur_tex_coords[5]) * 0.198596;
    out_color += texture(color_texture, blur_tex_coords[6]) * 0.175713;
    out_color += texture(color_texture, blur_tex_coords[7]) * 0.121703;
    out_color += texture(color_texture, blur_tex_coords[8]) * 0.065984;
    out_color += texture(color_texture, blur_tex_coords[9]) * 0.028002;
    out_color += texture(color_texture, blur_tex_coords[10]) * 0.0093;
}
