#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_UI_BUTTON_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_UI_BUTTON_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include <glm/glm.hpp>

#include <cstdint>
#include <functional>

namespace sfa
{

/// \brief Give an entity button properties.
///
/// \author Felix Hommel
/// \date 1/29/2026
struct UIButtonComponent : public IComponent
{
    /// \brief Describe in what kind of state a button is currently in.
    ///
    /// \author Felix Hommel
    /// \date 2/24/2026
    enum class ButtonState : std::uint8_t
    {
        Normal,
        Hovered,
        Pressed
    };

    glm::vec3 standardColor;
    float hoverFactor{ DEFAULT_BUTTON_HOVER_FACTOR };
    float pressFactor{ DEFAULT_BUTTON_PRESS_FACTOR };

    std::function<void()> onClick;
    float pressCooldownMax;
    float cooldownTimer{ 0.f };
    ButtonState state{ ButtonState::Normal };

private:
    static constexpr auto DEFAULT_BUTTON_HOVER_FACTOR{ 0.9f };
    static constexpr auto DEFAULT_BUTTON_PRESS_FACTOR{ 0.8f };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_UI_BUTTON_COMPONENT_HPP

