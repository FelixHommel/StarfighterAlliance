#ifndef INTERNAL_PARTICLE_GENERATOR_HPP
#define INTERNAL_PARTICLE_GENERATOR_HPP

#include "Shader.hpp"
#include "Texture.hpp"

#include <vector>

struct Particle
{
    glm::vec2 position{ 0.f };
    glm::vec2 velocity{ 0.f };
    glm::vec4 color{ 1.f };
    float life{ 0.f };
};

class ParticleGenerator
{
public:
    ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount);
    ~ParticleGenerator();

    ParticleGenerator(const ParticleGenerator&) = default;
    ParticleGenerator(ParticleGenerator&&) = delete;
    ParticleGenerator& operator=(const ParticleGenerator&) = default;
    ParticleGenerator& operator=(ParticleGenerator&&) = delete;

    // void update(float dt, GameObject &object, unsigned int newParticles, const glm::vec2& offset = glm::vec2(0.0f, 0.0f));
    void draw();

private:
    Shader m_shader;
    Texture2D m_texture;

    std::vector<Particle> m_particles;
    unsigned int m_amount;
    unsigned int m_lastUsedParticle;

    unsigned int m_vao;

    unsigned int firstUnusedParticle();
    // void respawnParticle(Particle &particle, GameObject &object, const glm::vec2& offset = glm::vec2(0.0f, 0.0f));
};

#endif //! INTERNAL_PARTICLE_GENERATOR_HPP
