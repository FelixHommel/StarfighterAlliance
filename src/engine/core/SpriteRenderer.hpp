#ifndef SFA_SRC_ENGINE_CORE_SPRITE_RENDERER_HPP
#define SFA_SRC_ENGINE_CORE_SPRITE_RENDERER_HPP

#include "Shader.hpp"
#include "Texture.hpp"

#include "glm/glm.hpp"

#include <array>
#include <cstddef>
#include <memory>

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
    /// \brief Create a new \ref SpriteRenderer
    ///
    /// \param shader the \ref Shader that will be used to render this quad
    explicit SpriteRenderer(std::shared_ptr<Shader> shader);
    ~SpriteRenderer();

    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer(SpriteRenderer&&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(SpriteRenderer&&) = delete;

    /// \brief Prepare the renderer for drawing the next frame.
    ///
    /// \param projection the projection matrix
    void beginFrame(const glm::mat4& projection);

    /// \brief Draw a textured quad to the screen
    ///
    /// The order of actions within the function is important, especially the translation, rotation, and scale order.
    ///     1. translate - bring the quad to the correct position
    ///     2. rotate - rotate the quad to the correct angle
    ///     3. scale - scale the quad to the correct size
    ///
    /// Note that the actual coordinates of all the quads are the same. The position on the screen is computed with matrix
    /// transformations, which achieves the same visual results.
    ///
    /// \param texture the texture of the quad
    /// \param position the position of the quad on screen
    /// \param size(optional) the size of the quad on screen
    /// \param rotate(optional) the rotation of the quad
    /// \param color(optional) the color of the quad. If a color and texture is supplied the two will be blended to produce the final look
    void draw(
        std::shared_ptr<Texture2D> texture,
        const glm::vec2& position,
        const glm::vec2& scale = DEFAULT_DRAW_SCALE,
        float rotate = DEFAULT_ROTATION,
        const glm::vec3& color = DEFAULT_COLOR
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
    static constexpr auto DEFAULT_DRAW_SCALE{ glm::vec2(10.f) };
    static constexpr auto DEFAULT_ROTATION{ 0.f };
    static constexpr auto DEFAULT_COLOR{ glm::vec3(1.f) };

    std::shared_ptr<Shader> m_shader;
    unsigned int m_quadVAO{ 0 };
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_SPRITE_RENDERER_HPP

