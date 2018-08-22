#include <engine.hpp>
#include <utils.hpp>
#include <post-processing.hpp>

FBO::FBO(int width, int height)
{
    m_w = width;
    m_h = height;
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glGenTextures(1, &m_colorbuffer);
    glBindTexture(GL_TEXTURE_2D, m_colorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorbuffer, 0);
    if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)) {
        printf("couldn't create fbo\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
    glDeleteFramebuffers(1, &m_id);
}

void FBO::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, m_w, m_h);
}

void FBO::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, game->screen_width, game->screen_height);
}
