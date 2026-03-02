#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TEXT_FIELD_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TEXT_FIELD_SYSTEM_HPP

#include "ecs/ComponentRegistry.hpp"

#include <glm/glm.hpp>

namespace sfa
{

struct UIInputState
{
    glm::vec2 mousePos{ 0.f };
    bool leftMousePressed{ false };
    bool leftMouseJustPressed{ false };

    std::string textInput;
    bool backspacePressed{ false };
    bool deletePressed{ false };
    bool moveCaretLeftPressed{ false };
    bool moveCaretRightPressed{ false };
    bool submitPressed{ false };
};

class UITextFieldSystem
{
public:
    static void update(ComponentRegistry& registry, float dt, const UIInputState& input);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_UI_TEXT_FIELD_SYSTEM_HPP

