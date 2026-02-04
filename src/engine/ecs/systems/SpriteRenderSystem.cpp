#include "SpriteRenderSystem.hpp"

#include "core/Shader.hpp"
#include "core/SpriteRenderer.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TransformComponent.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace sfa
{

SpriteRenderSystem::SpriteRenderSystem(std::shared_ptr<Shader> shader)
    : m_renderer{ std::make_unique<SpriteRenderer>(std::move(shader)) }
{}

void SpriteRenderSystem::render(const ComponentRegistry& components, const glm::mat4& projection)
{
    const auto& transforms{ components.getComponentArray<TransformComponent>() };
    const auto& sprites{ components.getComponentArray<SpriteComponent>() };

    std::vector<EntityID> renderables;
    for(std::size_t i{ 0 }; i < transforms.size(); ++i)
    {
        if(const auto entity{ transforms.entityAtIndex(i) }; sprites.contains(entity))
            renderables.emplace_back(entity);
    }

    std::ranges::sort(renderables, [&](EntityID a, EntityID b) {
        return sprites.get(a).renderLayer < sprites.get(b).renderLayer;
    });

    m_renderer->beginFrame(projection);
    for(const auto entity : renderables)
    {
        const auto& transform{ transforms.get(entity) };
        const auto& sprite{ sprites.get(entity) };

        m_renderer->draw(
            sprite.texture, transform.position, sprite.size * transform.scale, transform.rotation, sprite.color
        );
    }
}

} // namespace sfa

