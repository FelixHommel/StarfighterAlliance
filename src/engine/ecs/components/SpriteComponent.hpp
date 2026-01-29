#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_SPRITE_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_SPRITE_COMPONENT_HPP

#include "core/Texture.hpp"
#include "ecs/components/IComponent.hpp"

#include "glm/glm.hpp"

#include <memory>

namespace sfa
{

/// \brief Allow entities to have Sprite properties.
///
/// Give texture, size, color, and renderLayer to an entity
///
/// \author Felix Hommel
/// \date 1/26/2026
struct SpriteComponent : public IComponent
{
    std::shared_ptr<Texture2D> texture;
    glm::vec2 size{ glm::vec2(1.f) };
    glm::vec3 color{ glm::vec3(1.f) };
    unsigned int renderLayer{ 0 };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_SPRITE_COMPONENT_HPP

