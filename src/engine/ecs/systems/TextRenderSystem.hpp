#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_TEXT_RENDER_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_TEXT_RENDER_SYSTEM_HPP

#include "core/Shader.hpp"
#include "core/TextRenderer.hpp"
#include "ecs/ComponentRegistry.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace sfa
{

/// \brief The \ref TextRenderSystem is responsible to render text.
///
/// \author Felix Hommel
/// \date 1/26/2026
class TextRenderSystem
{
public:
    TextRenderSystem(std::shared_ptr<Shader> shader);
    ~TextRenderSystem() = default;

    TextRenderSystem(const TextRenderSystem&) = delete;
    TextRenderSystem& operator=(const TextRenderSystem&) = delete;
    TextRenderSystem(TextRenderSystem&&) = delete;
    TextRenderSystem& operator=(TextRenderSystem&&) = delete;

    /// \brief Render capable entities.
    ///
    /// Capable entities are all entities that have a \ref TransformComponent and \ref TextComponent.
    ///
    /// \param components \ref ComponentRegistry that maintains all the components
    /// \param projection projection matrix
    void render(const ComponentRegistry& components, const glm::mat4& projection);

private:
    std::unique_ptr<TextRenderer> m_renderer;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_TEXT_RENDER_SYSTEM_HPP

