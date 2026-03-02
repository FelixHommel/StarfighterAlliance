#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_UI_TEXT_FIELD_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_UI_TEXT_FIELD_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include <cstddef>
#include <functional>
#include <string>

namespace sfa
{

/// \brief Give Text field properties to an entity.
///
/// \author Felix Hommel
/// \date 3/1/2026
struct UITextFieldComponent : public IComponent
{
    std::string value;
    std::string placeholder;

    bool focused{ false };
    bool showCaret{ true };

    std::size_t caretPosition{ 0 };
    float blinkTimer{ 0.f };
    float blinkInterval{ 0.5f };

    std::function<void(const std::string&)> onSubmit;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_UI_TEXT_FIELD_COMPONENT_HPP

