
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
    m_width = 0;
    m_height = 0;
};

// deallocate
Texture::~Texture()
{
};

Texture::Texture(std::string path)
{
    // generate texture object
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

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
    }
    m_width = surface->w;
    m_height = surface->h;
    GLint mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
    CheckGLError();
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}
