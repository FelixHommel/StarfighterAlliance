#ifndef SFA_SRC_ENGINE_CORE_PARTICLE_GENERATOR_HPP
#define SFA_SRC_ENGINE_CORE_PARTICLE_GENERATOR_HPP

#include "Shader.hpp"
#include "Texture.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <vector>

namespace sfa
{

/// \brief Representation of a singular particle.
///
/// \author Felix Hommel
/// \date 1/24/2026
struct Particle
{
    glm::vec2 position{ glm::vec2(0.f) };
    glm::vec2 velocity{ glm::vec2(0.f) };
    glm::vec4 color{ glm::vec4(1.f) };
    float life{ 0.f };
};

/// \brief System that can be used to spawn particle effects.
///
/// \author Felix Hommel
/// \date 1/24/2026
class ParticleGenerator
{
public:
    ParticleGenerator(std::shared_ptr<Shader> shader, std::shared_ptr<Texture2D> texture, std::size_t amount);
    ~ParticleGenerator();

    ParticleGenerator(const ParticleGenerator&) = delete;
    ParticleGenerator(ParticleGenerator&&) = delete;
    ParticleGenerator& operator=(const ParticleGenerator&) = delete;
    ParticleGenerator& operator=(ParticleGenerator&&) = delete;

    // void update(float dt, GameObject &object, unsigned int newParticles, const glm::vec2& offset = glm::vec2(0.0f, 0.0f));
    /// \brief Draw all particles.
    ///
    /// Set up the OpenGL state, then draw all the particles that are alive and finally reset the OpenGL state to what
    /// it was before.
    void draw();

private:
    static constexpr std::size_t PARTICLE_QUAD_VERTICES{ 6 };
    static constexpr std::size_t PARTICLE_QUAD_VERTICES_ATTRIBUTES{ 4 };
    static constexpr std::array<float, PARTICLE_QUAD_VERTICES * PARTICLE_QUAD_VERTICES_ATTRIBUTES> particle_quad = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture2D> m_texture;

    std::vector<Particle> m_particles;
    std::size_t m_lastUsedParticle{ 0 };

    unsigned int m_vao{ 0 };

    std::size_t firstUnusedParticle();
    // void respawnParticle(Particle &particle, GameObject &object, const glm::vec2& offset = glm::vec2(0.0f, 0.0f));
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_PARTICLE_GENERATOR_HPP

