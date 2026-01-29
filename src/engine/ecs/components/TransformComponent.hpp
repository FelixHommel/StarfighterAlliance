#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include "glm/glm.hpp"

namespace sfa
{

/// \brief Allowing entities to have transform properties.
///
/// Give position, rotation, and scale properties to an entity.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct TransformComponent : public IComponent
{
    glm::vec2 position{ glm::vec2(0.f) };
    float rotation{ 0.f };
    glm::vec2 scale{ glm::vec2(1.f) };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

