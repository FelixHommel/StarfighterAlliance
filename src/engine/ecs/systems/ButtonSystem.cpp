#include "ButtonSystem.hpp"

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/ButtonComponent.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TransformComponent.hpp"

#include <glm/glm.hpp>

#include <cstddef>

namespace sfa
{

void ButtonSystem::update(ComponentRegistry& registry, float dt, const glm::vec2& mousePos, bool mousePressed)
{
    const auto& transforms{ registry.getComponentArray<TransformComponent>() };
    auto& sprites{ registry.getComponentArray<SpriteComponent>() };
    auto& buttons{ registry.getComponentArray<ButtonComponent>() };

    for(std::size_t i{ 0 }; i < buttons.size(); ++i)
    {
        const auto entity{ buttons.entityAtIndex(i) };
        auto& button{ buttons.get(entity) };
        auto& sprite{ sprites.get(entity) };
        const auto& transform{ transforms.get(entity) };

        if(button.cooldownTimer > 0.f)
            button.cooldownTimer -= dt;

        glm::vec2 min{ transform.position };
        glm::vec2 max{ transform.position + sprite.size };

        bool isHover{ mousePos.x >= min.x && mousePos.x <= max.x && mousePos.y >= min.y && mousePos.y <= max.y };

        if(isHover)
        {
            if(mousePressed && button.cooldownTimer <= 0.f)
            {
                button.activeColor = button.standardColor * button.pressFactor;

                if(button.onClick)
                    button.onClick();

                button.cooldownTimer = button.pressCooldownMax;
            }
            else
                button.activeColor = button.standardColor * button.hoverFactor;
        }
        else
            button.activeColor = button.standardColor;

        sprite.color = button.activeColor;
    }
}

} // namespace sfa

