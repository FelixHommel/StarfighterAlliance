#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_LAYOUT_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_LAYOUT_SYSTEM_HPP

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/LayoutComponent.hpp"
#include "ecs/components/TransformComponent.hpp"

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
    /// \brief Update all vertical layout elements.
    ///
    /// \param registry \ref ComponentRegistry containing all the components
    /// \param layout The \ref LayoutComponent that is updated
    /// \param parentTransform The \ref TransformComponent of the parent \ref LayoutComponent
    static void updateVerticalLayout(ComponentRegistry& registry, const LayoutComponent& layout, const TransformComponent& parentTransform);
    /// \brief Update all horizontal layout elements.
    ///
    /// \param registry \ref ComponentRegistry containing all the components
    /// \param layout The \ref LayoutComponent that is updated
    /// \param parentTransform The \ref TransformComponent of the parent \ref LayoutComponent
    static void updateHorizontalLayout(ComponentRegistry& registry, const LayoutComponent& layout, const TransformComponent& parentTransform);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_LAYOUT_SYSTEM_HPP

