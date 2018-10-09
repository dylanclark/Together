#include <post-processing.hpp>
#include <utils.hpp>
#include <loader.hpp>

PostProcessor::PostProcessor(std::string shader_name, int width, int height)
{
    m_shader = ResourceManager::get_shader(shader_name);
    m_w = width;
    m_h = height;
    m_name = shader_name;

    m_vao = Loader::load_to_postprocess_vao();
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
    } else if (m_name == "heat") {
        m_shader.set_float("time", (float) SDL_GetTicks() / 1000.0);
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
