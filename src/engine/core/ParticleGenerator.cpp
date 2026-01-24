#include "ParticleGenerator.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include "glad/gl.h"

#include <array>
#include <cstddef>
#include <memory>
#include <utility>

namespace sfa
{

ParticleGenerator::ParticleGenerator(std::shared_ptr<Shader> shader, std::shared_ptr<Texture2D> texture, std::size_t amount)
    : m_shader(std::move(shader)), m_texture(std::move(texture)), m_particles(amount)
{
    glGenVertexArrays(1, &m_vao);
    unsigned int vbo{};
    glGenBuffers(1, &vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, particle_quad.size() * sizeof(float), particle_quad.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        PARTICLE_QUAD_VERTICES_ATTRIBUTES,
        GL_FLOAT,
        GL_FALSE,
        PARTICLE_QUAD_VERTICES_ATTRIBUTES * sizeof(float),
        nullptr
    );
    glBindVertexArray(0);
}

ParticleGenerator::~ParticleGenerator()
{
    glDeleteVertexArrays(1, &m_vao);
}

/// \brief Needs to be optimized for the specific GameObject type.
// void ParticleGenerator::update(float dt, GameObject &object, unsigned int newParticles, const glm::vec2& offset)
// {
//     for (std::size_t i{ 0 }; i < newParticles; ++i)
//     {
//         int unusedParticle = firstUnusedParticle();
//         respawnParticle(m_particles[unusedParticle], object, offset);
//     }
//
//     for (std::size_t i{ 0 }; i < m_particles.size(); ++i)
//     {
//         Particle &p = m_particles[i];
//         p.life -= dt;
//         if (p.life > 0.0f)
//         {
//             p.position -= p.velocity * dt;
//             p.color.a -= dt * 2.5f;
//         }
//     }
// }

void ParticleGenerator::draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_shader->use();
    for(Particle particle : m_particles)
    {
        if(particle.life > 0.0f)
        {
            m_shader->setVector2f("offset", particle.position);
            m_shader->setVector4f("color", particle.color);
            m_texture->bind();
            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, PARTICLE_QUAD_VERTICES);
            glBindVertexArray(0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/// \brief Determine which is the first particle that is currently unused (i.e., its life is 0 or less).
///
/// \returns index of the first unused particle spot
std::size_t ParticleGenerator::firstUnusedParticle()
{
    for(std::size_t i{ m_lastUsedParticle }; i < m_particles.size(); ++i)
    {
        if(m_particles[i].life <= 0.0f)
        {
            m_lastUsedParticle = i;
            return i;
        }
    }

    for(std::size_t i{ 0 }; i < m_lastUsedParticle; ++i)
    {
        if(m_particles[i].life <= 0.0f)
        {
            m_lastUsedParticle = i;
            return i;
        }
    }

    m_lastUsedParticle = 0;
    return 0;
}

/// \brief Needs to be optimized for the specific GameObject type.
// void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, const glm::vec2& offset)
// {
//     float random = ((rand() % 100) - 50) / 10.0f;
//     float rColor = 0.5f + ((rand() % 100) / 100.0f);
//     particle.position = object.Position + random + offset;
//     particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
//     particle.life = 1.0f;
//     particle.velocity = object.Velocity * 0.1f;
// }

} // namespace sfa

