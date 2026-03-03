#include "UIRenderSystem.hpp"

#include "core/SpriteRenderer.hpp"
#include "core/TextRenderer.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <glad/gl.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace sfa
{

UIRenderSystem::UIRenderSystem(
    std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<TextRenderer> textRenderer
)
    : m_spriteRenderer(std::move(spriteRenderer)), m_textRenderer(std::move(textRenderer))
{}

void UIRenderSystem::render(ComponentRegistry& registry)
{
    const auto& transforms{ registry.getComponentArray<UITransformComponent>() };
    const auto& sprites{ registry.getComponentArray<SpriteComponent>() };
    const auto& texts{ registry.getComponentArray<TextComponent>() };

    std::vector<EntityID> drawables;
    for(std::size_t i{ 0 }; i < transforms.size(); ++i)
    {
        const auto entity{ transforms.entityAtIndex(i) };
        if(transforms.contains(entity) && (sprites.contains(entity) || texts.contains(entity)))
            drawables.push_back(entity);
    }

    std::ranges::sort(drawables, [&sprites, &texts](EntityID lhs, EntityID rhs) {
        const auto lhsLayer{ sprites.contains(lhs) ? sprites.get(lhs).renderLayer : texts.get(lhs).renderLayer };
        const auto rhsLayer{ sprites.contains(rhs) ? sprites.get(rhs).renderLayer : texts.get(rhs).renderLayer };

        return lhsLayer < rhsLayer;
    });

    // NOLINTNEXTLINE(readability-magic-numbers): 4 for viewport dimensions
    std::array<int, 4> viewport{ 0, 0, 1, 1 };
    glGetIntegerv(GL_VIEWPORT, viewport.data());

    const float width{ static_cast<float>(viewport[2]) };
    const float height{ static_cast<float>(viewport[3]) };
    const glm::mat4 projection{ glm::ortho(0.f, width, height, 0.f, -1.f, 1.f) };

    m_spriteRenderer->beginFrame(projection);
    m_textRenderer->beginFrame(projection);

    for(const auto entity : drawables)
    {
        const auto& transform{ transforms.get(entity) };

        if(sprites.contains(entity))
        {
            const auto& sprite{ sprites.get(entity) };
            m_spriteRenderer->draw(sprite.texture, transform.worldPosition, transform.size, 0.f, sprite.color);
        }

        if(texts.contains(entity))
        {
            const auto& text{ texts.get(entity) };

            glm::vec2 textPosition{ transform.worldPosition + text.offset };
            if(text.centerInTransform)
            {
                const auto bounds{ m_textRenderer->measureBounds(text.content, glm::vec2(text.scale)) };
                textPosition = transform.worldPosition + ((transform.size - bounds.size) * 0.5f) + text.offset;
            }

            m_textRenderer->render(text.content, textPosition, glm::vec2(text.scale), text.color);
        }
    }
}

} // namespace sfa

