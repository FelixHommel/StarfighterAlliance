#include "LayoutSystem.hpp"

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UILayoutComponent.hpp"
#include "ecs/components/UILayoutElementComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <glm/glm.hpp>

#include <cstddef>

namespace sfa
{

void LayoutSystem::update(ComponentRegistry& registry)
{
    const auto& hierarchies{ registry.getComponentArray<UIHierarchyComponent>() };
    const auto& layouts{ registry.getComponentArray<UILayoutComponent>() };
    const auto& elements{ registry.getComponentArray<UILayoutElementComponent>() };
    auto& transforms{ registry.getComponentArray<UITransformComponent>() };

    for(std::size_t i{ 0 }; i < layouts.size(); ++i)
    {
        const auto entity{ layouts.entityAtIndex(i) };
        const auto& layout{ layouts.get(entity) };

        if(!hierarchies.contains(entity) || !transforms.contains(entity))
            continue;

        const auto& hierachy{ hierarchies.get(entity) };
        auto& parentTransform{ transforms.get(entity) };

        if(layout.type == UILayoutComponent::Type::Vertical)
            updateVerticalLayout(layout, hierachy, parentTransform, transforms, elements);
        else
            updateHorizontalLayout(layout, hierachy, parentTransform, transforms, elements);
    }
}

/// \brief Update all vertical layout elements.
///
void LayoutSystem::updateVerticalLayout(
    const UILayoutComponent& layout,
    const UIHierarchyComponent& hierarchy,
    UITransformComponent& parentTransform,
    ComponentArray<UITransformComponent>& transforms,
    const ComponentArray<UILayoutElementComponent>& elements
)
{
    const float usableWidth{ parentTransform.size.x - (2.f * layout.padding.x) };
    float currentY{ layout.padding.y };

    for(const auto child : hierarchy.children)
    {
        if(!transforms.contains(child))
            continue;

        auto& childTransform{ transforms.get(child) };

        float preferredHeight{ childTransform.size.y };
        if(elements.contains(child))
            preferredHeight = elements.get(child).preferredSize.y;

        childTransform.localPosition = glm::vec2{ layout.padding.x, currentY };

        childTransform.size = glm::vec2{ usableWidth, preferredHeight };

        currentY += preferredHeight + layout.spacing;
    }
}

/// \brief Update all horizontal layout elements.
///
void LayoutSystem::updateHorizontalLayout(
    const UILayoutComponent& layout,
    const UIHierarchyComponent& hierarchy,
    UITransformComponent& parentTransform,
    ComponentArray<UITransformComponent>& transforms,
    const ComponentArray<UILayoutElementComponent>& elements
)
{
    const float usableHeight{ parentTransform.size.y - (2.f * layout.padding.y) };
    float currentX{ layout.padding.x };

    for(const auto child : hierarchy.children)
    {
        if(!transforms.contains(child))
            continue;

        auto& childTransform{ transforms.get(child) };

        float preferredWidth{ childTransform.size.x };
        if(elements.contains(child))
            preferredWidth = elements.get(child).preferredSize.x;

        childTransform.localPosition = glm::vec2{ currentX, layout.padding.y };

        childTransform.size = glm::vec2{ preferredWidth, usableHeight };

        currentX += preferredWidth + layout.spacing;
    }
}

} // namespace sfa

