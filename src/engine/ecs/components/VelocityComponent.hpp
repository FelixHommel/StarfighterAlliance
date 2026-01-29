#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_VELOCITY_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_VELOCITY_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include "glm/glm.hpp"

namespace sfa
{

/// \brief Allow a component to move with velocity.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct VelocityComponent : public IComponent
{
    glm::vec2 linear{ glm::vec2(0.f) };
    float angular{ 0.f };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_VELOCITY_COMPONENT_HPP

