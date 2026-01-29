#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_SPRITE_RENDER_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_SPRITE_RENDER_SYSTEM_HPP

#include "core/SpriteRenderer.hpp"
#include "core/Shader.hpp"
#include "ecs/ComponentManager.hpp"

#include "glm/glm.hpp"

#include <memory>

namespace sfa
{

/// \brief The \ref SpriteRenderSystem is responsible to render entities that are sprites.
///
/// In order to be rendered by the \ref SpriteRenderSystem, an entity needs to have a \ref SpriteComponent.
///
/// \author Felix Hommel
/// \date 1/26/2026
class SpriteRenderSystem
{
public:
    SpriteRenderSystem(std::shared_ptr<Shader> shader);
    ~SpriteRenderSystem() = default;

    SpriteRenderSystem(const SpriteRenderSystem&) = delete;
    SpriteRenderSystem& operator=(const SpriteRenderSystem&) = delete;
    SpriteRenderSystem(SpriteRenderSystem&&) = delete;
    SpriteRenderSystem& operator=(SpriteRenderSystem&&) = delete;

    /// \brief Render capable entities.
    ///
    /// An entity is capable if it has a \ref SpriteComponent.
    ///
    /// \param components const-ref to a \ref ComponentManager that maintains the components
    /// \param projection the projection matrix
    void render(const ComponentManager& components, const glm::mat4& projection);

private:
    std::unique_ptr<SpriteRenderer> m_renderer;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_SPRITE_RENDER_SYSTEM_HPP

