#include "SpriteRenderer.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include "glad/gl.h"
#include "glm/ext/matrix_transform.hpp"

#include <array>
#include <memory>
#include <utility>

namespace sfa
{

SpriteRenderer::SpriteRenderer(std::shared_ptr<Shader> shader) : m_shader(std::move(shader))
{
    glGenVertexArrays(1, &m_quadVAO);
    unsigned int VBO{};
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glVertexAttribPointer(0, SPRITE_VERTEX_ATTRIBUTES, GL_FLOAT, GL_FALSE, SPRITE_VERTEX_ATTRIBUTES * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void SpriteRenderer::beginFrame(const glm::mat4& projection)
{
    m_shader->setMatrix4("projection", projection, true);
}

void SpriteRenderer::draw(
    std::shared_ptr<Texture2D> texture, const glm::vec2& position, const glm::vec2& scale, float rotate, const glm::vec3& color
)
{
    m_shader->use();
    glm::mat4 model{ glm::mat4(1.f) };

    // NOTE: Step 1: Translate
    model = glm::translate(model, glm::vec3(position, 0.f));

    // NOTE: Step 2: Rotate
    // With the translation the rotation will happen around the center point of the sprite instead of top left corner
    if(rotate != 0.f)
    {
        model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.f));
    }

    // NOTE: Step 3: Scale
    model = glm::scale(model, glm::vec3(scale, 1.f));

    m_shader->setMatrix4("model", model);
    m_shader->setVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, SPRITE_VERTICES);

    glBindVertexArray(0);
}

} // namespace sfa

