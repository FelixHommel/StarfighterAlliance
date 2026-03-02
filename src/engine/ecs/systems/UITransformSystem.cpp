#include "UITransformSystem.hpp"

#include "ecs/ComponentArray.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
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

        if(hierarchies.get(entity).parent == NULL_ENTITY && transforms.contains(entity))
            propagate(entity, transforms, hierarchies);
    }
}

/// \brief Recursively apply transforms to elements and their child elements.
///
/// \param entity the current entity that is processed
/// \param transforms a \ref ComponentArray with all \ref UITransformComponent stored in it
/// \param hierarchies q \ref ComponentArray with all \ref UIHierarchyComponent stored in it
void UITransformSystem::propagate(EntityID entity, ComponentArray<UITransformComponent>& transforms, const ComponentArray<UIHierarchyComponent>& hierarchies)
{
    if(!hierarchies.contains(entity) || !transforms.contains(entity))
        return;

    const auto& hierarchy{ hierarchies.get(entity) };
    auto& transform{ transforms.get(entity) };

    if(hierarchy.parent != NULL_ENTITY)
        transform.worldPosition = transforms.get(hierarchy.parent).worldPosition + transform.localPosition;
    else
        transform.worldPosition = transform.localPosition;

    for(const auto& child : hierarchy.children)
    {
        if(hierarchies.contains(child) && transforms.contains(child))
            propagate(child, transforms, hierarchies);
    }
}

} // namespace sfa

