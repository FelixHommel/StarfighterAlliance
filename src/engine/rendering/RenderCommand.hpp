#ifndef SFA_SRC_ENGINE_RENDERING_RENDER_COMMAND_HPP
#define SFA_SRC_ENGINE_RENDERING_RENDER_COMMAND_HPP

#include "core/Shader.hpp"
#include "entities/components/Sprite.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <variant>

namespace sfa
{

/// \brief Submission data for a 2D sprite.
///
/// \author Felix Hommel
/// \date 11/11/2026
struct SpriteSubmission
{
    // GL resources
    SpriteAppearance appearance;
    std::shared_ptr<Shader> shader{ nullptr };

    // Sprite component
    glm::vec2 size{};
    unsigned int renderLayer{};

    // Translation component
    glm::vec2 position{};
    glm::vec2 scale{};
    float rotation{};
};

/// \brief Submission data for text.
///
/// \author Felix Hommel
/// \date 11/11/2026
struct TextSubmission
{
    // TODO: Reference to font
    std::string text;

    glm::vec4 color{};
    unsigned int renderLayer{};

    glm::vec2 position{};
    glm::vec2 scale{};
};

using RenderCommand = std::variant<SpriteSubmission, TextSubmission>;

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_RENDER_COMMAND_HPP

