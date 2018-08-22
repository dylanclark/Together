#include <post-processing.hpp>
#include <utils.hpp>

LevelGraphicsPipeline::LevelGraphicsPipeline()
{
    m_fbo = new FBO(game->screen_width, game->screen_height);

    // set up vao
    float vertices[] = {
        -1.0, 1.0,
        1.0, 1.0,
        1.0, -1.0,
        -1.0, -1.0
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    m_processors["invertor"] = PostProcessor("invertor");
    m_processors["bright_filter"] = PostProcessor("bright_filter", game->screen_width/2, game->screen_height/2);
    m_processors["vblur"] = PostProcessor("vblur", game->screen_width/2, game->screen_height/2);
    m_processors["hblur"] = PostProcessor("hblur", game->screen_width/2, game->screen_height/2);
    m_combiner = Combiner("combiner");
}

void LevelGraphicsPipeline::begin()
{
    m_fbo->bind();
}

void LevelGraphicsPipeline::render()
{
    m_fbo->unbind();
    GLuint output1 = m_fbo->get_texture();
    GLuint output2 = m_processors["bright_filter"].render(output1);
    output2 = m_processors["hblur"].render(output2);
    output2 = m_processors["vblur"].render(output2);
    GLuint output = m_combiner.render(output1, output2);
    Shader m_shader = ResourceManager::get_shader("display");
    m_shader.use();
    m_shader.set_int("color_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, output);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
