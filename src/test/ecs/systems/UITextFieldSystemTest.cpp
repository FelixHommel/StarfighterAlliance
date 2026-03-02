#include "ecs/systems/UITextFieldSystem.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/UITextFieldComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <glm/glm.hpp>

#include <gtest/gtest.h>

namespace
{

constexpr sfa::EntityID FIELD_ENTITY{ 1 };

constexpr glm::vec2 FIELD_POSITION{ 10.f };
constexpr glm::vec2 FIELD_SIZE{ 100.f, 30.f };

constexpr auto BLINK_INTERVAL{ 100.f };

constexpr glm::vec2 CLICK_POS{ 20.f };

constexpr auto DELTA_TIME{ 0.016f };

} // namespace

namespace sfa::testing
{

TEST(UITextFieldSystemTest, FoxusAndTypingUpdatesTextAndCaret)
{
    ComponentRegistry registry;

    registry.addComponent<UITransformComponent>(
        FIELD_ENTITY, { .localPosition = glm::vec2(0.f), .worldPosition = FIELD_POSITION, .size = FIELD_SIZE }
    );
    registry.addComponent<TextComponent>(
        FIELD_ENTITY,
        { .content = "", .offset = glm::vec2(0.f), .scale = 1.f, .color = glm::vec3(1.f), .renderLayer = 0 }
    );
    registry.addComponent<UITextFieldComponent>(
        FIELD_ENTITY,
        { .value = "",
          .placeholder = "Type...",
          .focused = false,
          .showCaret = true,
          .caretPosition = 0,
          .blinkTimer = 0.f,
          .blinkInterval = BLINK_INTERVAL }
    );

    UIInputState clickInside{};
    clickInside.mousePos = CLICK_POS;
    clickInside.leftMouseJustPressed = true;

    UITextFieldSystem::update(registry, DELTA_TIME, clickInside);

    auto& field{ registry.getComponent<UITextFieldComponent>(FIELD_ENTITY) };
    auto& text{ registry.getComponent<TextComponent>(FIELD_ENTITY) };

    EXPECT_TRUE(field.focused);

    UIInputState typing{};
    typing.textInput = "abc";

    UITextFieldSystem::update(registry, DELTA_TIME, typing);

    EXPECT_STREQ("abc", field.value.c_str());
    EXPECT_EQ(3u, field.caretPosition);
    EXPECT_STREQ("abc|", text.content.c_str());
}

} // namespace sfa::testing

