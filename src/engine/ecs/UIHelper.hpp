#ifndef SFA_SRC_ENGINE_ECS_UI_HELPER_HPP
#define SFA_SRC_ENGINE_ECS_UI_HELPER_HPP

#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/UIButtonComponent.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UILayoutElementComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <string>
#include <utility>

namespace
{

constexpr auto DEFAULT_TEXT_SCALE{ 0.8f };
constexpr auto DEFAULT_BUTTON_PRESS_COOL_DOWN{ 0.2f };

} // namespace

namespace sfa::ui
{

void addButton(
    sfa::ComponentRegistry& registry,
    const sfa::EntityID entity,
    const sfa::EntityID parent,
    const glm::vec2& size,
    std::string text,
    const glm::vec3& color,
    const std::function<void()>& onClick
)
{
    registry.addComponent<sfa::UITransformComponent>(
        entity,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = size,
        }
    );
    registry.addComponent<sfa::UIHierarchyComponent>(entity, { .parent = parent, .children = {} });
    registry.addComponent<sfa::UILayoutElementComponent>(
        entity,
        {
            .preferredSize = size,
            .flexGrow = 0,
        }
    );
    registry.addComponent<sfa::SpriteComponent>(
        entity,
        {
            .texture = nullptr,
            .size = size,
            .color = color,
            .renderLayer = 0,
        }
    );
    registry.addComponent<sfa::TextComponent>(
        entity,
        {
            .content = std::move(text),
            .offset = glm::vec2(0.f),
            .scale = ::DEFAULT_TEXT_SCALE,
            .color = glm::vec3(1.f),
            .centerInTransform = true,
            .renderLayer = 1,
        }
    );
    registry.addComponent<sfa::UIButtonComponent>(
        entity,
        {
            .standardColor = color,
            .onClick = onClick,
            .pressCooldownMax = ::DEFAULT_BUTTON_PRESS_COOL_DOWN,
        }
    );
}

} // namespace sfa::ui

#endif // !SFA_SRC_ENGINE_ECS_UI_HELPER_HPP
