#include <post-processing.hpp>
#include <utils.hpp>

Combiner::Combiner(std::string shader_name, int width, int height)
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

GLuint Combiner::render(GLuint texture1, GLuint texture2)
{
    // bind our fbo and use our shader
    m_fbo->bind();
    m_shader.use();

    // bind the texture and draw it
    m_shader.set_int("texture1", 0);
    m_shader.set_int("texture2", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // wrap up and return
    m_fbo->unbind();
    return m_fbo->get_texture();
}
