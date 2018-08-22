#include <stdlib.h>
#include <math.h>
#include <objects.hpp>

#define PI 3.14159265

Light::Light(int x, int y, float strength, SDL_Color color)
{
    m_x = x;
    m_y = y;
    m_strength = strength;
    m_color = color;

    // set up period of light fluctuation
    float random = ((float) rand() / (float) RAND_MAX);
    m_period = random * 1000 - 500 + BASE_LIGHT_PERIOD;
}

float Light::get_strength()
{
    float output = m_strength + LIGHT_FLUCT_AMPL * sin((float) (SDL_GetTicks() * 2 * PI) / (float) m_period);
    return output;
}
