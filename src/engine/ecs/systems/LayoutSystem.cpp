#include "LayoutSystem.hpp"

#include "ecs/ComponentArray.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UILayoutComponent.hpp"
#include "ecs/components/UILayoutElementComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <cstddef>
#include <vector>

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

        const auto& hierarchy{ hierarchies.get(entity) };
        auto& parentTransform{ transforms.get(entity) };

        if(layout.type == UILayoutComponent::Type::Vertical)
            updateVerticalLayout(layout, hierarchy, parentTransform, transforms, elements);
        else
            updateHorizontalLayout(layout, hierarchy, parentTransform, transforms, elements);
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
    const float usableWidth{ std::max(0.f, parentTransform.size.x - (2.f * layout.padding.x)) };
    const float usableHeight{ std::max(0.f, parentTransform.size.y - (2.f * layout.padding.y)) };

    std::vector<EntityID> layoutChildren;
    layoutChildren.reserve(hierarchy.children.size());

    float totalPreferredHeight{ 0.f };
    float totalFlexGrow{ 0.f };

    for(const auto child : hierarchy.children)
    {
        if(!transforms.contains(child))
            continue;

        layoutChildren.push_back(child);

        float preferredHeight{ transforms.get(child).size.y };
        if(elements.contains(child))
        {
            preferredHeight = elements.get(child).preferredSize.y;
            totalFlexGrow += std::max(0.f, elements.get(child).flexGrow);
        }

        totalPreferredHeight += preferredHeight;
    }

    if(layoutChildren.empty())
        return;

    const float totalSpacing{ layout.spacing * static_cast<float>(layoutChildren.size() - 1) };
    const float freeSpace{ std::max(0.f, usableHeight - totalPreferredHeight - totalSpacing) };

    float currentY{ layout.padding.y };
    for(const auto child : layoutChildren)
    {
        auto& childTransform{ transforms.get(child) };

        float preferredHeight{ childTransform.size.y };
        float flexGrow{ 0.f };
        if(elements.contains(child))
        {
            preferredHeight = elements.get(child).preferredSize.y;
            flexGrow = std::max(0.f, elements.get(child).flexGrow);
        }

        const float flexBonus{ totalFlexGrow > 0.f ? (freeSpace * (flexGrow / totalFlexGrow)) : 0.f };
        const float finalHeight{ preferredHeight + flexBonus };

        childTransform.localPosition = glm::vec2{ layout.padding.x, currentY };
        childTransform.size = glm::vec2{ usableWidth, finalHeight };

        currentY += finalHeight + layout.spacing;
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
    const float usableWidth{ std::max(0.f, parentTransform.size.x - (2.f * layout.padding.x)) };
    const float usableHeight{ std::max(0.f, parentTransform.size.y - (2.f * layout.padding.y)) };

    std::vector<EntityID> layoutChildren;
    layoutChildren.reserve(hierarchy.children.size());

    float totalPreferredWidth{ 0.f };
    float totalFlexGrow{ 0.f };
    for(const auto child : hierarchy.children)
    {
        if(!transforms.contains(child))
            continue;

        layoutChildren.push_back(child);

        float preferredWidth{ transforms.get(child).size.x };
        if(elements.contains(child))
        {
            preferredWidth = elements.get(child).preferredSize.x;
            totalFlexGrow += std::max(0.f, elements.get(child).flexGrow);
        }

        totalPreferredWidth += preferredWidth;
    }

    if(layoutChildren.empty())
        return;

    const float totalSpacing{ layout.spacing * static_cast<float>(layoutChildren.size() - 1) };
    const float freeSpace{ std::max(0.f, usableWidth - totalPreferredWidth - totalSpacing) };

    float currentX{ layout.padding.x };
    for(const auto child : layoutChildren)
    {
        auto& childTransform{ transforms.get(child) };

        float preferredWidth{ childTransform.size.x };
        float flexGrow{ 0.f };
        if(elements.contains(child))
        {
            preferredWidth = elements.get(child).preferredSize.x;
            flexGrow = std::max(0.f, elements.get(child).flexGrow);
        }

        const float flexBonus{ totalFlexGrow > 0.f ? (freeSpace * (flexGrow / totalFlexGrow)) : 0.f };
        const float finalWidth{ preferredWidth + flexBonus };

        childTransform.localPosition = glm::vec2{ currentX, layout.padding.y };
        childTransform.size = glm::vec2{ finalWidth, usableHeight };

        currentX += finalWidth + layout.spacing;
    }
}

} // namespace sfa

