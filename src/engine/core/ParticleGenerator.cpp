#include "ParticleGenerator.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include "glad/gl.h"

#include <array>

ParticleGenerator::ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount)
    : m_shader(shader), m_texture(texture), m_amount(amount), m_lastUsedParticle(0), m_vao(0)
{
    unsigned int VBO{};

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &VBO);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

    for(unsigned int i = 0; i < m_amount; ++i)
        m_particles.emplace_back();
}

ParticleGenerator::~ParticleGenerator()
{
    glDeleteVertexArrays(1, &m_vao);
}

/*
 *   @brief Needs to be optimiezed for the specific GameObject type
 */
// void ParticleGenerator::update(float dt, GameObject &object, unsigned int newParticles, const glm::vec2& offset)
// {
//     for (unsigned int i = 0; i < newParticles; ++i)
//     {
//         int unusedParticle = firstUnusedParticle();
//         respawnParticle(m_particles[unusedParticle], object, offset);
//     }
//
//     for (unsigned int i = 0; i < m_amount; ++i)
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
    m_shader.use();
    for(Particle particle : m_particles)
    {
        if(particle.life > 0.0f)
        {
            m_shader.setVector2f("offset", particle.position);
            m_shader.setVector4f("color", particle.color);
            m_texture.bind();
            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, PARTICLE_QUAD_VERTICES);
            glBindVertexArray(0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int ParticleGenerator::firstUnusedParticle()
{
    for(unsigned int i = m_lastUsedParticle; i < m_amount; ++i)
    {
        if(m_particles[i].life <= 0.0f)
        {
            m_lastUsedParticle = i;
            return i;
        }
    }

    for(unsigned int i = 0; i < m_lastUsedParticle; ++i)
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

/*
 *   @brief Needs to be optimiezed for the specific GameObject type
 */
// void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, const glm::vec2& offset)
// {
//     float random = ((rand() % 100) - 50) / 10.0f;
//     float rColor = 0.5f + ((rand() % 100) / 100.0f);
//     particle.position = object.Position + random + offset;
//     particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
//     particle.life = 1.0f;
//     particle.velocity = object.Velocity * 0.1f;
// }
