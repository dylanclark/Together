#include <vector>

class Particle
{
public:
    Particle(int x, int y, float life_length, float scale, float gravity);
    bool update();

private:
    int m_x, m_y;
    int m_xvel, m_yvel;

    float m_life_length;

    float m_scale;
    float m_gravity;
};

class ParticleSystem
{
public:
    ParticleSystem(
        int x, int y, int w, int h,
        float pps, float gravity, float scale, float scale_eps,
        float xvel, float xvel_eps, float yvel, float yvel_eps) :
        m_x(x), m_y(y), m_w(w), m_h(h), m_pps(pps), m_gravity(gravity),
        m_scale(scale), m_scale_eps(scale_eps), m_xvel(xvel), m_xvel_eps(xvel_eps),
        m_yvel(yvel), m_yvel_eps(yvel_eps) { }

    void update();
    void render_particles();

private:
    // particles can spawn anywhere in this rect
    int m_x, m_y;
    int m_w, m_h;
    // particles per second
    float m_pps;
    // particle size
    float m_scale;
    float m_scale_eps;
    // particle speed
    float m_xvel, m_yvel;
    float m_xvel_eps, m_yvel_eps;

    int m_gravity;

    std::vector<Particle> m_particles;
};
