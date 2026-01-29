#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_DAMAGE_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_DAMAGE_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

namespace sfa
{

/// \brief Allow entities to deal damage.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct DamageComponent : public IComponent
{
    int amount;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_DAMAGE_COMPONENT_HPP

