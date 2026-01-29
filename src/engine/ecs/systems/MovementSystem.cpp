#include "MovementSystem.hpp"

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/TransformComponent.hpp"
#include "ecs/components/VelocityComponent.hpp"

#include <cstddef>

namespace sfa
{

void MovementSystem::update(ComponentRegistry& components, float dt)
{
    auto& transforms{ components.getComponentArray<TransformComponent>() };
    auto& velocities{ components.getComponentArray<VelocityComponent>() };

    for(std::size_t i{ 0 }; i < transforms.size(); ++i)
    {
        auto entity{ transforms.entityAtIndex(i) };

        if(velocities.contains(entity))
        {
            auto& transform{ transforms.get(entity) };
            auto& velocity{ velocities.get(entity) };

            transform.position += velocity.linear * dt;
            transform.rotation += velocity.angular * dt;
        }
    }
}

} // namespace sfa

