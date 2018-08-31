#include <stdlib.h>
#include <math.h>
#include <objects.hpp>

#define PI 3.14159265

static const int BASE_LIGHT_PERIOD = 4000;
static const int LIGHT_FLUCT_AMPL = 10;
static const int TURN_ON_PERIOD = 500;
static const float OFF_FACTOR = 2.0;

Light::Light(int x, int y, float range, SDL_Color palette)
{
    m_x = x;
    m_y = y;
    m_range = range;
    m_palette = palette;

    // set up period of light fluctuation
    float random = ((float) rand() / (float) RAND_MAX);
    m_period = random * 1000 - 500 + BASE_LIGHT_PERIOD;
}

float Light::get_range()
{
    float output = m_range + LIGHT_FLUCT_AMPL * sin((float) (SDL_GetTicks() * 2 * PI) / (float) m_period);
    int x = SDL_GetTicks() - m_start_time;
    if (!on) {
        output /= OFF_FACTOR;
        output += (output*(OFF_FACTOR-1) * pow(2, -((float) x / (float) 200))) * (cos((float) (x * 2 * PI) / (float) TURN_ON_PERIOD));
    } else {
        output += (output/OFF_FACTOR * pow(2, -((float) x / (float) 200))) * (-cos((float) (x * 2 * PI) / (float) TURN_ON_PERIOD));
    }
    return output;
}

float Light::get_strength()
{
    float output = 1.0;
    int x = SDL_GetTicks() - m_start_time;
    if (!on) {
        output /= OFF_FACTOR;
        output += (output*(OFF_FACTOR-1) * pow(2, -((float) x / (float) 200))) * (cos((float) (x * 2 * PI) / (float) TURN_ON_PERIOD));
    } else {
        output += (output/OFF_FACTOR * pow(2, -((float) x / (float) 200))) * (-cos((float) (x * 2 * PI) / (float) TURN_ON_PERIOD));
    }
    return output;
}

void Light::turn_on()
{
    on = true;
    m_start_time = SDL_GetTicks();
}

void Light::turn_off()
{
    on = false;
    m_start_time = SDL_GetTicks();
}
