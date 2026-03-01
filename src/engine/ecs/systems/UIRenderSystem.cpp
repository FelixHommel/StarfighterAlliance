#include "UIRenderSystem.hpp"

#include "ecs/ECSUtility.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace sfa
{

UIRenderSystem::UIRenderSystem(std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<TextRenderer> textRenderer)
    : m_spriteRenderer(std::move(spriteRenderer))
    , m_textRenderer(std::move(textRenderer))
{}

void UIRenderSystem::render(ComponentRegistry& registry)
{
    const auto& transforms{ registry.getComponentArray<UITransformComponent>() };
    const auto& sprites{ registry.getComponentArray<SpriteComponent>() };
    const auto& texts{ registry.getComponentArray<TextComponent>() };

    std::vector<EntityID> drawables;
    for(std::size_t i{ 0 }; i < transforms.size(); ++i)
    {
        auto d{ transforms.entityAtIndex(i) };
        if(transforms.contains(d) && sprites.contains(d))
            drawables.emplace_back(std::move(d));
    }

    std::ranges::sort(drawables, [&sprites = sprites](EntityID lhs, EntityID rhs) {
        return sprites.get(lhs).renderLayer < sprites.get(rhs).renderLayer;
    });

    for(auto d : drawables)
    {
        const auto& transform{ transforms.get(d) };
        const auto& sprite{ sprites.get(d) };

        // TODO: complete rendering system
    }
}

} // namespace sfa

