
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header file
#include <textures.hpp>
#include <levels.hpp>
#include <char.hpp>
#include <engine.hpp>
#include <utils.hpp>

// initialize
Texture::Texture()
{
    width = 0;
    height = 0;
};

// deallocate
Texture::~Texture()
{
};

bool Texture::load_object(int w, int h, std::string path, SDL_Color* palette)
{
    // get object dimensions
    if (palette) {
        m_palette.r = palette->r;
        m_palette.g = palette->g;
        m_palette.b = palette->b;
    } else {
        m_palette.r = m_palette.g = m_palette.b = 255;
    }
    width = w;
    height = h;

    // generate texture object
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // we'll set some parameters for completeness
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // let's load the image
    SDL_Surface* surface = IMG_Load(("resources/textures/"+path).c_str());
    if (surface == NULL) {
        printf("Unable to load image %s! SDL error: %s\n",
               ("resources/textures/"+path).c_str(), SDL_GetError());
        return false;
    }
    tex_width = surface->w;
    tex_height = surface->h;
    GLint mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, mode, tex_width, tex_height, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);

    float tex_w = (float) width / (float) tex_width;
    float tex_h = (float) height / (float) tex_height;

    float vertices[] = {
        0.0f, 0.0f,            0.0f, 0.0f,
        (float) width, 0.0f,            tex_w, 0.0f,
        (float) width, (float) height,  tex_w, tex_h,
        0.0f, (float) height,  0.0f, tex_h
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint vertex_buffer_obj;
    GLuint element_buffer_obj;

    // first, we create the 2d object in model space
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vertex_buffer_obj);
    glGenBuffers(1, &element_buffer_obj);
    // make sure the array obj is bound so it knows what's up with the buffer objs
    glBindVertexArray(m_vao);

    // set up the buffer objs
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set vertex attributes
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    return true;
};

void Texture::render(int x, int y, SDL_Rect* clip, Camera* cam, int dir, int flip)
{
    // first, let's see if we should even bother
    SDL_Rect cam_rect = cam->get_display();
    SDL_Rect m_rect = {x, y, width, height};
    Vector repos;
    if (!check_collision(m_rect, cam_rect, &repos)) {
        return;
    }

    // we'll use our (only available) shader
    game->m_shader->use();

    // set model matrix - this tells us where our object is in the world space
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::translate(model, glm::vec3(width*dir, height*flip, 0.0f));
    model = glm::scale(model, glm::vec3(!dir - dir, !flip - flip, 1.0f));
    game->m_shader->set_float_mat4("model", model);

    // set view matrix - this tells us what part of the world we are looking at
    game->m_shader->set_float_mat4("view", cam->get_view());

    // set projection matrix - this determines the perspective with which we look at stuff
    game->m_shader->set_float_mat4("proj", cam->get_proj());

    // set texture clip matrix - this decides which piece of the texture to use
    float tex_x = (float) clip->x / (float) tex_width;
    float tex_y = (float) clip->y / (float) tex_height;
    glm::mat4 tex_clip;
    tex_clip = glm::translate(tex_clip, glm::vec3(tex_x, tex_y, 0.0f));
    game->m_shader->set_float_mat4("tex_clip", tex_clip);

    // set color
    glm::vec3 obj_color = glm::vec3(m_palette.r / (float) 255, m_palette.g / (float) 255, m_palette.b / (float) 255);
    game->m_shader->set_vec3("obj_color", obj_color);

    // set texture
    game->m_shader->set_int("m_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // draw that puppy!
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
