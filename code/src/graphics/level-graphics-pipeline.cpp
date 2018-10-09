#include <post-processing.hpp>
#include <loader.hpp>
#include <utils.hpp>

LevelGraphicsPipeline::LevelGraphicsPipeline()
{
    m_fbo = new FBO(game->screen_width, game->screen_height);

    m_vao = Loader::load_to_postprocess_vao();

    m_processors["invertor"] = PostProcessor("invertor");
    m_processors["ripple"] = PostProcessor("ripple");
    m_processors["bright_filter"] = PostProcessor("bright_filter",
                                                  game->screen_width/2,
                                                  game->screen_height/2);
    m_processors["vblur"] = PostProcessor("vblur", game->screen_width/2,
                                         game->screen_height/2);
    m_processors["hblur"] = PostProcessor("hblur", game->screen_width/2,
                                         game->screen_height/2);
    m_processors["heat"] = PostProcessor("heat");
    m_combiner = Combiner("combiner");
}

void LevelGraphicsPipeline::begin()
{
    m_fbo->bind();
}

void LevelGraphicsPipeline::render(int time_ms, int ripple_x, int ripple_y)
{
    m_fbo->unbind();
    GLuint output1 = m_fbo->get_texture();
    output1 = m_processors["ripple"].render_ripple(output1, time_ms, ripple_x, ripple_y);
    output1 = m_processors["heat"].render(output1);
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
