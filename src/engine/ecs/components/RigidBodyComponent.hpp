#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_RIGID_BODY_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_RIGID_BODY_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

namespace sfa
{

/// \brief Allow an entity to have rigid body properties.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct RigidBodyComponent : public IComponent
{
    float mass{ 1.f };
    float drag{ 0.f };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_RIGID_BODY_COMPONENT_HPP

