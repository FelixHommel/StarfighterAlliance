#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_UI_HIERARCHY_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_UI_HIERARCHY_COMPONENT_HPP

#include "ecs/ECSUtility.hpp"
#include "ecs/components/IComponent.hpp"

#include <vector>

namespace sfa
{

/// \brief Allow UI elements to be in a hierarchical structure.
///
/// \author Felix Hommel
/// \date 2/24/2026
struct UIHierarchyComponent : public IComponent
{
    EntityID parent{ NULL_ENTITY };
    std::vector<EntityID> children;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_UI_HIERARCHY_COMPONENT_HPP

