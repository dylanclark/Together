#include <string>
#include <glad.h>
#include <engine.hpp>

/***********/
/*   FBO   */
/***********/

class FBO
{
public:
    FBO(int width, int height);
    ~FBO();

    void bind();
    void unbind();

    GLuint get_texture() { return m_colorbuffer; }

private:
    int m_w, m_h;
    GLuint m_id;
    GLuint m_colorbuffer;
};

/**********************/
/*   POST PROCESSOR   */
/**********************/

class PostProcessor
{
public:
    PostProcessor() { }
    PostProcessor(std::string shader, int width = game->screen_width, int height = game->screen_height);
    ~PostProcessor() { }

    GLuint render(GLuint texture);
    GLuint render_ripple(GLuint texture, int time_ms, int ripple_x, int ripple_y);

private:
    int m_w, m_h;
    std::string m_name;
    Shader m_shader;
    FBO* m_fbo;
    GLuint m_vao;
};

class Combiner
{
public:
    Combiner() { }
    Combiner(std::string shader, int width = game->screen_width, int height = game->screen_height);
    ~Combiner() { }

    GLuint render(GLuint texture1, GLuint texture2);

private:
    int m_w, m_h;
    std::string m_name;
    Shader m_shader;
    FBO* m_fbo;
    GLuint m_vao;
};

/**************************/
/*   GRAPHICS PIPELINES   */
/**************************/

class LevelGraphicsPipeline
{
public:
    LevelGraphicsPipeline();
    ~LevelGraphicsPipeline() { }

    void begin();
    void render(int time_ms, int ripple_x, int ripple_y);

private:
    std::map<std::string, PostProcessor> m_processors;
    Combiner m_combiner;
    FBO* m_fbo;
    GLuint m_vao;
};
