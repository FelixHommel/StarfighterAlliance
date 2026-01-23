#ifndef SFA_SRC_ENGINE_CORE_SPRITE_RENDERER_HPP
#define SFA_SRC_ENGINE_CORE_SPRITE_RENDERER_HPP

#include "Shader.hpp"
#include "Texture.hpp"

#include <array>
#include <cstddef>

namespace sfa
{

/// \brief Abstraction class over Rendering in OpenGL.
///
/// The class abstracts rendering in a way, where only the configuration of the sprite has to be
/// supplied at render time and then the Renderer executes all OpenGL functions required to render
/// a textured quad.
///
/// \author Felix Hommel
/// \date 11/17/2024
class SpriteRenderer
{
public:
    SpriteRenderer(const Shader& shader);
    ~SpriteRenderer();

    SpriteRenderer(const SpriteRenderer&) = default;
    SpriteRenderer(SpriteRenderer&&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = default;
    SpriteRenderer& operator=(SpriteRenderer&&) = delete;

    void draw(
        const Texture2D& texture,
        const glm::vec2& position,
        const glm::vec2& size = DEFAULT_DRAW_SIZE,
        float rotate = 0.f,
        const glm::vec3& color = glm::vec3(1.f)
    );

private:
    static constexpr std::size_t SPRITE_VERTICES{ 6 };
    static constexpr std::size_t SPRITE_VERTEX_ATTRIBUTES{ 4 };
    static constexpr std::array<float, SPRITE_VERTICES * SPRITE_VERTEX_ATTRIBUTES> vertices{
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    static constexpr auto DEFAULT_DRAW_SIZE{ glm::vec2(10.f) };

    Shader m_shader;
    unsigned int m_quadVAO;
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_SPRITE_RENDERER_HPP

