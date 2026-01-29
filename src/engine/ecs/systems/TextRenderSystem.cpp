#include "TextRenderSystem.hpp"

#include "core/Shader.hpp"
#include "core/TextRenderer.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/TransformComponent.hpp"

#include "glm/glm.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace sfa
{

TextRenderSystem::TextRenderSystem(std::shared_ptr<Shader> shader)
    : m_renderer{ std::make_unique<TextRenderer>(std::move(shader)) }
{}

void TextRenderSystem::render(const ComponentManager& components, const glm::mat4& projection)
{
    const auto* transforms{ components.getComponentArray<TransformComponent>() };
    const auto* texts{ components.getComponentArray<TextComponent>() };

    std::vector<EntityID> renderables;
    for(std::size_t i{ 0 }; i < transforms->size(); ++i)
    {
        if(const auto entity{ transforms->entityAtIndex(i) }; texts->contains(entity))
            renderables.emplace_back(entity);
    }

    std::ranges::sort(renderables, [&](EntityID a, EntityID b){
        return texts->get(a).renderLayer < texts->get(b).renderLayer;
    });

    m_renderer->beginFrame(projection);
    for(const auto entity : renderables)
    {
        const auto& transform{ transforms->get(entity) };
        const auto& text{ texts->get(entity) };

        const glm::vec2 renderPos{ transform.position + text.offset };
        m_renderer->render(text.content, renderPos, transform.scale, text.color);
    }
}

} // namespace sfa

