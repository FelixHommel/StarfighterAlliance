#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_CIRCLE_COLLIDER_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_CIRCLE_COLLIDER_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include "glm/glm.hpp"

namespace sfa
{

/// \brief Allow a circle shaped entity to collide with other entities.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct CircleColliderComponent : public IComponent
{
    float radius;
    glm::vec2 offset{ glm::vec2(0.f) };
};

}

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_CIRCLE_COLLIDER_COMPONENT_HPP

