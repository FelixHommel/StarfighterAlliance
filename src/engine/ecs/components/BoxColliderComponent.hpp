#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_BOX_COLLIDER_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_BOX_COLLIDER_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include "glm/glm.hpp"

namespace sfa
{

/// \brief Allow an entity to collide with other entities.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct BoxColliderComponent : public IComponent
{
    glm::vec2 size;
    glm::vec2 offset{ glm::vec2(0.f) };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_BOX_COLLIDER_COMPONENT_HPP

