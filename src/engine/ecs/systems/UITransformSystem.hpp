#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TRANSFORM_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TRANSFORM_SYSTEM_HPP

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

namespace sfa
{

/// \brief This system is responsible for calculating the positions of child layout elements.
///
/// \author Felix Hommel
/// \date 2/24/2026
class UITransformSystem
{
public:
    /// \brief Update the positions of UI components.
    ///
    /// \param registry the \ref ComponentRegistry to access the UI elements
    static void update(ComponentRegistry& registry);

private:
    static void propagate(
        EntityID entity,
        ComponentArray<UITransformComponent>& transforms,
        const ComponentArray<UIHierarchyComponent>& hierarchies
    );
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TRANSFORM_SYSTEM_HPP

