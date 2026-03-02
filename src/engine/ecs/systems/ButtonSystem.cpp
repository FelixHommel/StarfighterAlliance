#include "ButtonSystem.hpp"

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/UIButtonComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <glm/glm.hpp>

#include <cstddef>

namespace sfa
{

void ButtonSystem::update(ComponentRegistry& registry, float dt, const glm::vec2& mousePos, bool mousePressed)
{
    const auto& transforms{ registry.getComponentArray<UITransformComponent>() };
    auto& sprites{ registry.getComponentArray<SpriteComponent>() };
    auto& buttons{ registry.getComponentArray<UIButtonComponent>() };

    for(std::size_t i{ 0 }; i < buttons.size(); ++i)
    {
        const auto entity{ buttons.entityAtIndex(i) };

        if(!transforms.contains(entity) || !sprites.contains(entity) || !buttons.contains(entity))
            continue;

        const auto& transform{ transforms.get(entity) };
        auto& button{ buttons.get(entity) };
        auto& sprite{ sprites.get(entity) };

        if(button.cooldownTimer > 0.f)
            button.cooldownTimer -= dt;

        const glm::vec2& min{ transform.worldPosition };
        const glm::vec2 max{ transform.worldPosition + transform.size };

        if(mousePos.x >= min.x && mousePos.x <= max.x && mousePos.y >= min.y && mousePos.y <= max.y)
        {
            if(mousePressed && button.cooldownTimer <= 0.f)
            {
                button.state = UIButtonComponent::ButtonState::Pressed;

                if(button.onClick)
                    button.onClick();

                button.cooldownTimer = button.pressCooldownMax;
            }
            else
                button.state = UIButtonComponent::ButtonState::Hovered;
        }
        else
            button.state = UIButtonComponent::ButtonState::Normal;

        sprite.color = determineButtonColor(button);
    }
}

glm::vec3 ButtonSystem::determineButtonColor(const UIButtonComponent& button)
{
    // NOTE: Technically could be cached in the UIButtonComponent, so that color is not calculated every time.

    if(button.state == UIButtonComponent::ButtonState::Pressed)
        return button.standardColor * button.pressFactor;
    if(button.state == UIButtonComponent::ButtonState::Hovered)
        return button.standardColor * button.hoverFactor;

    return button.standardColor;
}

} // namespace sfa

