#include "BasicTextPipeline.hpp"

#include "core/FontMap.hpp"
#include "core/Shader.hpp"
#include "rendering/RenderCommand.hpp"

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>

namespace
{

constexpr std::size_t GLYPH_VERTICES{ 6 };
constexpr std::size_t GLYPH_VERTEX_ATTRIBUTES{ 4 };
constexpr auto ADVANCE_BITSHIFT{ 6 };

}

namespace sfa
{

BasicTextPipeline::BasicTextPipeline(std::shared_ptr<Shader> shader) : m_shader(std::move(shader))
{
    m_shader->setUniform("text", 0);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER, sizeof(float) * ::GLYPH_VERTICES * ::GLYPH_VERTEX_ATTRIBUTES, nullptr, GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0, ::GLYPH_VERTEX_ATTRIBUTES, GL_FLOAT, GL_FALSE, ::GLYPH_VERTEX_ATTRIBUTES * sizeof(float), nullptr
    );
    glEnableVertexAttribArray(0);
}

BasicTextPipeline::~BasicTextPipeline()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void BasicTextPipeline::beginFrame(const glm::mat4& projection) const
{
    m_shader->setUniform("projection", projection, true);
}

void BasicTextPipeline::draw(const TextSubmission& submission) const
{
    m_shader->setUniform("textColor", submission.color, true);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    float x{ submission.position.x };
    for(auto c{ submission.text.cbegin() }; c != submission.text.cend(); c++)
    {
        // TODO: Add a sort of clamping mechanism here to ensure that characters are valid
        const auto ch{ submission.font->at(*c) };

        const float xpos{ x + (static_cast<float>(ch.bearing.x) * submission.scale.x) };
        const float ypos{ submission.position.y
                          + (static_cast<float>(submission.font->at('H').bearing.y - ch.bearing.y)
                             * submission.scale.y) };

        const float w{ static_cast<float>(ch.size.x) * submission.scale.x };
        const float h{ static_cast<float>(ch.size.y) * submission.scale.y };
        std::array<float, ::GLYPH_VERTICES* ::GLYPH_VERTEX_ATTRIBUTES> vertices{
            xpos, ypos + h, 0.0f, 1.0f, xpos + w, ypos,     1.0f, 0.0f, xpos,     ypos, 0.0f, 0.0f,
            xpos, ypos + h, 0.0f, 1.0f, xpos + w, ypos + h, 1.0f, 1.0f, xpos + w, ypos, 1.0f, 0.0f
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());

        glDrawArrays(GL_TRIANGLES, 0, ::GLYPH_VERTICES);

        // NOTE: Bitshift by 6 == 2^6, advance is 1/64 of a pixel
        x += static_cast<float>(ch.advance >> ADVANCE_BITSHIFT) * submission.scale.x;
    }
}

} // namespace sfa

