#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_LAYOUT_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_LAYOUT_SYSTEM_HPP

#include "ecs/ComponentArray.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UILayoutComponent.hpp"
#include "ecs/components/UILayoutElementComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

namespace sfa
{

/// \brief The \ref LayoutSystem is responsible for updating the position of layout elements in the grid.
///
/// \author Felix Hommel
/// \date 1/29/2026
class LayoutSystem
{
public:
    /// \brief Update all layout elements.
    ///
    /// \param registry \ref ComponentRegistry containing all the components
    static void update(ComponentRegistry& registry);

private:
    static void updateVerticalLayout(
        const UILayoutComponent& layout,
        const UIHierarchyComponent& hierarchy,
        UITransformComponent& parentTransform,
        ComponentArray<UITransformComponent>& transforms,
        const ComponentArray<UILayoutElementComponent>& elements
    );
    static void updateHorizontalLayout(
        const UILayoutComponent& layout,
        const UIHierarchyComponent& hierarchy,
        UITransformComponent& parentTransform,
        ComponentArray<UITransformComponent>& transforms,
        const ComponentArray<UILayoutElementComponent>& elements
    );
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_LAYOUT_SYSTEM_HPP

