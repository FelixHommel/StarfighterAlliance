#include "BasicSpritePipeline.hpp"

#include "core/Shader.hpp"
#include "core/TextureView.hpp"
#include "rendering/RenderCommand.hpp"

#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <array>
#include <cstddef>
#include <memory>
#include <utility>
#include <variant>

namespace
{

struct SpriteVertex
{
    glm::vec2 position;
    glm::vec2 uv;
};

constexpr std::size_t SPRITE_VERTICES{ 6 };
constexpr std::size_t SPRITE_VERTEX_ATTRIBUTES{ 4 };
constexpr std::array<unsigned char, 4> WHITE_PIXEL{ 255, 255, 255, 255 };
constexpr std::array<float, SPRITE_VERTICES * SPRITE_VERTEX_ATTRIBUTES> VERTICES{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                                                                                  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                                                                  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                                                                  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f };

} // namespace

namespace sfa
{

BasicSpritePipeline::BasicSpritePipeline(std::shared_ptr<Shader> shader) : m_shader{ std::move(shader) }
{
    glGenVertexArrays(1, &m_quadVAO);
    GLuint vbo{};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, ::VERTICES.size() * sizeof(float), ::VERTICES.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glVertexAttribPointer(
        0, ::SPRITE_VERTEX_ATTRIBUTES, GL_FLOAT, GL_FALSE, ::SPRITE_VERTEX_ATTRIBUTES * sizeof(float), nullptr
    );
    glEnableVertexAttribArray(0);

    glDeleteBuffers(1, &vbo);

    glGenTextures(1, &m_fallbackTexture);
    glBindTexture(GL_TEXTURE_2D, m_fallbackTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA8, GL_UNSIGNED_BYTE, ::WHITE_PIXEL.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

BasicSpritePipeline::~BasicSpritePipeline()
{
    glDeleteVertexArrays(1, &m_quadVAO);

    if(m_fallbackTexture != 0)
        glDeleteTextures(1, &m_fallbackTexture);
}

void BasicSpritePipeline::beginFrame(const glm::mat4& projection)
{
    m_shader->setUniform("projection", projection, true);
}

void BasicSpritePipeline::draw(const SpriteSubmission& submission)
{
    // TODO: Let the sprite calculate its model matrix

    glm::mat4 model{ glm::mat4(1.f) };

    // NOTE: Step 1: Translate
    model = glm::translate(model, glm::vec3(submission.position, 0.f));

    // NOTE: Step 2: Rotate
    // With the translation the rotation will happen around the center point of the sprite instead of top left corner
    if(submission.rotation != 0.f)
    {
        const auto rotationTranslation{ glm::vec3(0.5f * submission.scale.x, 0.5f * submission.scale.y, 0.f) };

        model = glm::translate(model, rotationTranslation);
        model = glm::rotate(model, glm::radians(submission.rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, -rotationTranslation);
    }

    // NOTE: Step 3: Scale
    model = glm::scale(model, glm::vec3(submission.scale, 1.f));

    m_shader->setUniform("model", model, true);

    bindAppearance(submission);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, ::SPRITE_VERTICES);
}

void BasicSpritePipeline::bindAppearance(const SpriteSubmission& submission)
{
    glActiveTexture(GL_TEXTURE0);
    std::visit([this](const auto& appearance) { applyAppearanceImpl(appearance); }, submission.appearance);
}

void BasicSpritePipeline::applyAppearanceImpl(const glm::vec3& appearance)
{
    m_shader->setUniform("spriteColor", appearance);
    glBindTexture(GL_TEXTURE_2D, m_fallbackTexture);
}

void BasicSpritePipeline::applyAppearanceImpl(TextureView appearance)
{
    static constexpr auto COLOR_WHITE{ glm::vec3(1.f) };

    m_shader->setUniform("spriteColor", COLOR_WHITE);
    glBindTexture(appearance.target, appearance.id);
}

} // namespace sfa

