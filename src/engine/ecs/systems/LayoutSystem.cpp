#include "LayoutSystem.hpp"

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/LayoutComponent.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TransformComponent.hpp"

#include <cstddef>

namespace sfa
{

void LayoutSystem::update(ComponentRegistry& registry)
{
    const auto& layouts{ registry.getComponentArray<LayoutComponent>() };
    const auto& transforms{ registry.getComponentArray<TransformComponent>() };

    for(std::size_t i{ 0 }; i < layouts.size(); ++i)
    {
        const auto entity{ layouts.entityAtIndex(i) };
        const auto& layout{ layouts.get(entity) };
        const auto& parentTransform{ transforms.get(entity) };

        if(layout.type == LayoutComponent::Type::Vertical)
            updateVerticalLayout(registry, layout, parentTransform);
        else if(layout.type == LayoutComponent::Type::Horizontal)
            updateHorizontalLayout(registry, layout, parentTransform);
    }
}

void LayoutSystem::updateVerticalLayout(
    ComponentRegistry& registry, const LayoutComponent& layout, const TransformComponent& parentTransform
)
{
    const auto& sprites{ registry.getComponentArray<SpriteComponent>() };
    auto& transforms{ registry.getComponentArray<TransformComponent>() };

    float currentY{ parentTransform.position.y + layout.padding.y };

    for(auto child : layout.children)
    {
        auto& childTransform{ transforms.get(child) };
        childTransform.position.x = parentTransform.position.x + layout.padding.x;
        childTransform.position.y = currentY;

        if(sprites.contains(child))
            currentY += sprites.get(child).size.y + layout.spacing;
    }
}

void LayoutSystem::updateHorizontalLayout(
    ComponentRegistry& registry, const LayoutComponent& layout, const TransformComponent& parentTransform
)
{
    const auto& sprites{ registry.getComponentArray<SpriteComponent>() };
    auto& transforms{ registry.getComponentArray<TransformComponent>() };

    float currentX{ parentTransform.position.x + layout.padding.x };

    for(auto child : layout.children)
    {
        auto& childTransform{ transforms.get(child) };
        childTransform.position.x = currentX;
        childTransform.position.y = parentTransform.position.y + layout.padding.y;

        if(sprites.contains(child))
            currentX += sprites.get(child).size.x + layout.spacing;
    }
}

} // namespace sfa

