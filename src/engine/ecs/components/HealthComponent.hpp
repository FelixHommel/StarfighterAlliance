#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_HEALTH_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_HEALTH_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

namespace sfa
{

/// \brief Allow entities to have health.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct HealthComponent : public IComponent
{
    int current;
    int max;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_HEALTH_COMPONENT_HPP

