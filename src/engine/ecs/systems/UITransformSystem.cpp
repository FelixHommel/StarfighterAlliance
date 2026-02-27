#include "UITransformSystem.hpp"

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <cstddef>

namespace sfa
{

void UITransformSystem::update(ComponentRegistry& registry)
{
    const auto& hierarchies{ registry.getComponentArray<UIHierarchyComponent>() };
    auto& transforms{ registry.getComponentArray<UITransformComponent>() };

    for(std::size_t i{ 0 }; i < hierarchies.size(); ++i)
    {
        const auto entity{ hierarchies.entityAtIndex(i) };
        const auto& hierarchy{ hierarchies.get(entity) };
        const auto& parentTransform{ transforms.get(entity) };

        for(const auto& child : hierarchy.children)
        {
            auto& childTransform{ transforms.get(child) };

            childTransform.worldPosition = parentTransform.worldPosition + childTransform.localPosition;
        }
    }
}

} // namespace sfa

