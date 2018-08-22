// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad.h>
#include <glm/gtc/matrix_transform.hpp>

// include header file
#include <levels.hpp>
#include <textures.hpp>
#include <tiles.hpp>
#include <utils.hpp>

/************/
/*   TILE   */
/************/

bool tiletype_isslope(TileType type)
{
    return (type == TILE_SLOPE_2_DOWN_A || type == TILE_SLOPE_2_DOWN_B ||
            type == TILE_SLOPE_2_UP_A || type == TILE_SLOPE_2_UP_B);
}

Tile::Tile(int x, int y, TileType tile_type)
{
    // set position
    m_rect.x = x;
    m_rect.y = y;

    // set collision box size
    m_rect.w = TILE_WIDTH;
    m_rect.h = TILE_WIDTH;

    // set tile type
    m_type = tile_type;
}

/***************/
/*   TILESET   */
/***************/

void setup_vertices(float* result, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h)
{
    // top left point
    result[0] = x*TILE_WIDTH;
    result[1] = y*TILE_WIDTH;
    result[2] = tex_x;
    result[3] = tex_y;
    // top right point
    result[4] = x*TILE_WIDTH + TILE_WIDTH;
    result[5] = y*TILE_WIDTH;
    result[6] = tex_x + tex_w;
    result[7] = tex_y;
    // bottom right point
    result[8] = x*TILE_WIDTH + TILE_WIDTH;
    result[9] = y*TILE_WIDTH + TILE_WIDTH;
    result[10] = tex_x + tex_w;
    result[11] = tex_y + tex_h;
    // bottom left point
    result[12] = x*TILE_WIDTH;
    result[13] = y*TILE_WIDTH + TILE_WIDTH;
    result[14] = tex_x;
    result[15] = tex_y + tex_h;
}

Tileset::Tileset(std::vector<Tile> &tiles, int x, int y, int w, int h, SDL_Color palette)
{
    m_tiles = tiles;
    // true coords
    m_x = x;
    m_y = y;
    // grid coords
    m_w = w;
    m_h = h;
    m_palette.r = palette.r;
    m_palette.g = palette.g;
    m_palette.b = palette.b;

    // generate texture object
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // we'll set some parameters for completeness
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // let's load the image
    SDL_Surface* surface = IMG_Load("resources/textures/tiles.png");
    if (surface == NULL) {
        printf("Unable to load image %s! SDL error: %s\n",
               "resources/textures/tiles.png", SDL_GetError());
    }
    tex_width = surface->w;
    tex_height = surface->h;
    GLint mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, tex_width, tex_height, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);

    // generate normal_map texture object
    glGenTextures(1, &m_normalmap);
    glBindTexture(GL_TEXTURE_2D, m_normalmap);

    // we'll set some parameters for completeness
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // let's load the normal_map
    SDL_Surface* normal_surface = IMG_Load("resources/textures/tiles-normal.png");
    if (normal_surface == NULL) {
        printf("Unable to load image %s! SDL error: %s\n",
               "resources/textures/tiles-normal.png", SDL_GetError());
    }
    mode = GL_RGB;
    if (normal_surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, tex_width, tex_height, 0, mode, GL_UNSIGNED_BYTE, normal_surface->pixels);
    SDL_FreeSurface(normal_surface);

    // we need 4 vertices for every quad, 4 coords per vertex, w*h quads total
    float* vertices = (float*) malloc(sizeof(float) * 4 * 4 * (w*h));

    // 6 indices for every quad, w*h quads total
    unsigned int* indices = (unsigned int*) malloc(sizeof(unsigned int) * 6 * w*h);
    unsigned int indices_pattern[6] = {
        0, 1, 2,    // top right triangle
        0, 2, 3     // bottom left triangle
    };

    for (int i = 0; i < w*h; i++) {
        // first, set up indices
        for (int j = 0; j < 6; j++) {
            indices[i*6 + j] = indices_pattern[j];
            indices_pattern[j] += 4;
        }
        float tex_x = (float) (tiles[i].get_type() * TILE_WIDTH * 2) / (float) tex_width;
        float tex_y = 0.0f;
        float tex_w = (float) TILE_WIDTH / (float) tex_width;
        float tex_h = (float) TILE_WIDTH / (float) tex_height;
        // set up vertices
        setup_vertices(vertices + i*16, i % w, i / w, tex_x, tex_y, tex_w, tex_h);
    }
    for (int i = 0; i < w*h*16; i++) {
    }

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16*m_w*m_h, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6*m_w*m_h, indices, GL_STATIC_DRAW);

    // set vertex attributes
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    free(indices);
    free(vertices);
}

void Tileset::render(Camera* cam, std::vector<Light> lights, bool active_color)
{
    Shader m_shader = ResourceManager::get_shader("level_normal");
    m_shader.use();

    // set model matrix - this tells us where our object is in the world space
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(m_x, m_y, 0.0f));
    m_shader.set_float_mat4("model", model);

    // set view matrix - this tells us what part of the world we are looking at
    m_shader.set_float_mat4("view", cam->get_view());

    // set projection matrix - this determines the perspective with which we look at stuff
    m_shader.set_float_mat4("proj", cam->get_proj());

    // set texture clip matrix - this decides which piece of the texture to use
    float tex_x_offset = (float) ((active_color == 1) * TILE_WIDTH) / (float) tex_width;
    glm::mat4 tex_clip;
    tex_clip = glm::translate(tex_clip, glm::vec3(tex_x_offset, 0.0f, 0.0f));
    m_shader.set_float_mat4("tex_clip", tex_clip);

    // set color
    float obj_r = (float) m_palette.r / (float) 255;
    float obj_g = (float) m_palette.g / (float) 255;
    float obj_b = (float) m_palette.b / (float) 255;
    glm::vec3 obj_color = glm::vec3(obj_r, obj_g, obj_b);
    m_shader.set_vec3("obj_color", obj_color);

    // set up lights
    m_shader.set_int("num_lights", lights.size());
    for (int i = 0; i < lights.size(); i++) {
        std::string number = std::to_string(i);
        m_shader.set_float("lights["+number+"].strength", lights[i].get_strength());
        m_shader.set_vec2("lights["+number+"].position", glm::vec2(lights[i].get_x(), lights[i].get_y()));
    }

    m_shader.set_int("m_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    m_shader.set_int("m_normalmap", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalmap);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6*m_w*m_h, GL_UNSIGNED_INT, 0);
}

