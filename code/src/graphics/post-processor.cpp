#include <post-processing.hpp>
#include <utils.hpp>

PostProcessor::PostProcessor(std::string shader_name, int width, int height)
{
    m_shader = ResourceManager::get_shader(shader_name);
    m_w = width;
    m_h = height;
    m_name = shader_name;

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

    m_fbo = new FBO(width, height);
}

GLuint PostProcessor::render(GLuint texture)
{
    // bind our fbo and use our shader
    m_shader.use();
    m_fbo->bind();

    // only for blurring
    if (m_name == "hblur" || m_name == "vblur") {
        m_shader.set_float("target_width", m_w);
        m_shader.set_float("target_height", m_h);
    }

    // bind the texture and draw it
    m_shader.set_int("color_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // wrap up and return
    m_fbo->unbind();
    return m_fbo->get_texture();
}

GLuint PostProcessor::render_ripple(GLuint texture, int time_ms, int ripple_x, int ripple_y)
{
    m_shader.use();
    m_fbo->bind();

    m_shader.set_float("timer", (float) time_ms / (float) 1000);
    m_shader.set_float("screen_x", (float) ripple_x / (float) game->screen_width);
    m_shader.set_float("screen_y", 1.0 - (float) ripple_y / (float) game->screen_height);

    // bind the texture and draw it
    m_shader.set_int("color_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // wrap up and return
    m_fbo->unbind();
    return m_fbo->get_texture();
}
