#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TRANSFORM_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TRANSFORM_SYSTEM_HPP

#include "ecs/ComponentRegistry.hpp"

namespace sfa
{

/// \brief This system is responsible for calculating the positions of child layout elements.
///
/// \author Felix Hommel
/// \date 2/24/2026
class UITransformSystem
{
public:
    static void update(ComponentRegistry& registry);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TRANSFORM_SYSTEM_HPP

