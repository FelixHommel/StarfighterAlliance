#include "UITextFieldSystem.hpp"

#include "ecs/ComponentRegistry.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/UITextFieldComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <cstddef>

namespace
{

bool isInside(const sfa::UITransformComponent& transform, const glm::vec2& point)
{
    const glm::vec2 min{ transform.worldPosition };
    const glm::vec2 max{ transform.worldPosition + transform.size };

    return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
}

}

namespace sfa
{

void UITextFieldSystem::update(ComponentRegistry& registry, float dt, const UIInputState& input)
{
    const auto& transforms{ registry.getComponentArray<UITransformComponent>() };
    auto& textFields{ registry.getComponentArray<UITextFieldComponent>() };
    auto& texts{ registry.getComponentArray<TextComponent>() };

    for(std::size_t i{ 0 }; i < textFields.size(); ++i)
    {
        const auto entity{ textFields.entityAtIndex(i) };

        if(!transforms.contains(entity) && !texts.contains(entity))
            continue;

        const auto& transform{ transforms.get(entity) };
        auto& field{ textFields.get(entity) };
        auto& text{ texts.get(entity) };

        if(input.leftMouseJustPressed)
            field.focused = isInside(transform, input.mousePos);

        field.caretPosition = std::min(field.caretPosition, field.value.size());

        if(field.focused)
        {
            if(input.moveCaretLeftPressed && field.caretPosition > 0)
                --field.caretPosition;
            if(input.moveCaretRightPressed && field.caretPosition < field.value.size())
                ++field.caretPosition;

            if(input.backspacePressed && field.caretPosition > 0)
            {
                field.value.erase(field.caretPosition - 1, 1);
                --field.caretPosition;
            }
            if(input.deletePressed && field.caretPosition < field.value.size())
                field.value.erase(field.caretPosition, 1);

            if(!input.textInput.empty())
            {
                field.value.insert(field.caretPosition, input.textInput);
                field.caretPosition += input.textInput.size();
            }

            if(input.submitPressed && field.onSubmit)
                field.onSubmit(field.value);

            field.blinkTimer += dt;
            if(field.blinkTimer >= field.blinkInterval)
            {
                field.blinkTimer = 0.f;
                field.showCaret = !field.showCaret;
            }
        }
        else
        {
            field.showCaret = false;
            field.blinkTimer = 0.f;
        }

        if(field.value.empty())
            text.content = field.placeholder;
        else
            text.content = field.value;

        if(field.focused && field.showCaret)
            text.content.insert(field.caretPosition, "|");
    }
}

} // namespace sfa

