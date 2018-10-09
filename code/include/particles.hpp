
class Particle
{
public:
    Particle(int x, int y, int life_length, float scale, float gravity);
    bool update();

private:
    int m_x, m_y;
    int m_xvel, m_yvel;

    float m_life_length;

    float m_scale;
    float m_gravity;
};

class ParticleEmitter
{
public:
    ParticleEmitter();
    void render_particles();

private:
    std::vector<Particle> m_particles;
};
