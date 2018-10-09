#include <glm/gtc/matrix_transform.hpp>
#include <engine.hpp>
#include <levels.hpp>
#include <utils.hpp>
#include <textures.hpp>
#include <loader.hpp>

Sprite::Sprite(Texture tex, Texture normal_tex, int w, int h, SDL_Color* palette)
{
    m_tex = tex;
    m_normalmap = normal_tex;
    m_width = w;
    m_height = h;
    if (palette) {
        m_palette = *palette;
    } else {
        m_palette.r = m_palette.g = m_palette.b = 255;
    }
    float tex_w = (float) m_width / (float) m_tex.get_width();
    float tex_h = (float) m_height / (float) m_tex.get_height();

    float vertices[] = {
        0.0f, 0.0f,            0.0f, 0.0f,
        (float) m_width, 0.0f,            tex_w, 0.0f,
        (float) m_width, (float) m_height,  tex_w, tex_h,
        0.0f, (float) m_height,  0.0f, tex_h
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    m_vao = Loader::load_to_vao(vertices, 16, indices, 6);
}

void Sprite::render(int x, int y, SDL_Rect* clip, Camera* cam, std::vector<Light> lights, int dir, int flip, bool is_light)
{
    // first, let's see if we should even bother
    SDL_Rect cam_rect = cam->get_display();
    SDL_Rect m_rect = {x, y, m_width, m_height};
    Vector repos;
    if (!check_collision(m_rect, cam_rect, &repos)) {
        return;
    }

    Shader m_shader;
    if (!is_light) {
        m_shader = ResourceManager::get_shader("level_normal");
    } else {
        m_shader = ResourceManager::get_shader("light");
    }
    m_shader.use();

    // set model matrix - this tells us where our object is in the world space
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::translate(model, glm::vec3(m_width*dir, m_height*flip, 0.0f));
    model = glm::scale(model, glm::vec3(!dir - dir, !flip - flip, 1.0f));
    m_shader.set_float_mat4("model", model);

    // set view matrix - this tells us what part of the world we are looking at
    m_shader.set_float_mat4("view", cam->get_view());

    // set projection matrix - this determines the perspective with which we look at stuff
    m_shader.set_float_mat4("proj", cam->get_proj());

    // set texture clip matrix - this decides which piece of the texture to use
    float tex_x = (float) clip->x / (float) m_tex.get_width();
    float tex_y = (float) clip->y / (float) m_tex.get_height();
    glm::mat4 tex_clip;
    tex_clip = glm::translate(tex_clip, glm::vec3(tex_x, tex_y, 0.0f));
    m_shader.set_float_mat4("tex_clip", tex_clip);

    // set color
    glm::vec3 obj_color = glm::vec3(m_palette.r / (float) 255, m_palette.g / (float) 255, m_palette.b / (float) 255);
    m_shader.set_vec3("obj_color", obj_color);

    if (!is_light) {
        // set up lights
        m_shader.set_int("num_lights", lights.size());
        for (int i = 0; i < lights.size(); i++) {
            std::string number = std::to_string(i);
            m_shader.set_float("lights["+number+"].range", lights[i].get_range());
            m_shader.set_float("lights["+number+"].strength", lights[i].get_strength());
            m_shader.set_vec2("lights["+number+"].position", glm::vec2(lights[i].get_x(), lights[i].get_y()));
        }
    }

    // set texture
    m_shader.set_int("m_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    m_tex.bind();
    m_shader.set_int("m_normalmap", 1);
    m_shader.set_int("flipped", flip);
    m_shader.set_int("dir", dir);
    glActiveTexture(GL_TEXTURE1);
    m_normalmap.bind();

    // draw that puppy!
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::render(int x, int y, int scr_w, int scr_h, SDL_Rect* clip)
{
    Shader m_shader = ResourceManager::get_shader("to_texture");
    m_shader.use();

    // set model matrix - this tells us where our object is in the world space
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    m_shader.set_float_mat4("model", model);

    // set projection matrix - this determines the perspective with which we look at stuff
    glm::mat4 proj = glm::ortho(0.0f, (float) scr_w, 0.0f, (float) scr_h);
    m_shader.set_float_mat4("proj", proj);

    // set texture clip matrix - this decides which piece of the texture to use
    float tex_x = (float) clip->x / (float) m_tex.get_width();
    float tex_y = (float) clip->y / (float) m_tex.get_height();
    glm::mat4 tex_clip;
    tex_clip = glm::translate(tex_clip, glm::vec3(tex_x, tex_y, 0.0f));
    m_shader.set_float_mat4("tex_clip", tex_clip);

    // set texture
    m_shader.set_int("m_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    m_tex.bind();

    // draw that puppy!
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
