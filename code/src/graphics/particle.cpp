#include <SDL2/SDL.h>
#include <particles.hpp>

Particle::Particle(int x, int y, float life_length, float scale, float gravity)
{
    m_x = x;
    m_y = y;
    m_xvel = 0;
    m_yvel = 0;
    m_life_length = (float) (SDL_GetTicks() / 1000.0) + life_length;
    m_scale = scale;
    m_gravity = gravity;
}

bool
Particle::update()
{
    float elapsed_life = (float) SDL_GetTicks() / 1000.0;
    m_yvel += m_gravity;
    m_x += m_xvel;
    m_y += m_yvel;
    return (elapsed_life < m_life_length);
}

void
ParticleSystem::render_particles()
{
    for (int i = 0; i < m_particles.size(); i++) {

    }
}

void
ParticleSystem::update()
{
    return;
}
