#ifndef SFA_SRC_ENGINE_UTILITY_INPUT_CONTROLLER_HPP
#define SFA_SRC_ENGINE_UTILITY_INPUT_CONTROLLER_HPP

#include "utility/userInput/InputEvent.hpp"

#include <cstddef>
#include <queue>
#include <unordered_map>
#include <utility>

namespace sfa
{

/// \brief The InputController is responsible for processing user input
///
/// \author Felix Hommel
/// \date 2/21/2026
class InputController
{
public:
    InputController() = default;
    ~InputController() = default;

    InputController(const InputController&) = delete;
    InputController& operator=(const InputController&) = delete;
    InputController(InputController&&) noexcept = delete;
    InputController& operator=(InputController&&) noexcept = delete;

    /// \brief Register the occurrence of a new \ref InputEvent.
    ///
    /// \param event the new \ref InputEvent
    void registerEvent(InputEvent event) { m_inputs.push(std::move(event)); }
    /// \brief Process the queued inputs.
    void processEventQueue();

    /// \brief Check if a certain key is currently pressed.
    /// \note Assume Release as state when there has not yet been a state change that was recorded.
    ///
    /// \param key the Key to be checked
    ///
    /// \returns *Press* if \p key is pressed, *Release* if not
    [[nodiscard]] InputAction isKeyPressed(Key key) const;
    /// \brief Check if a certain mouse button is currently pressed.
    /// \note Assume Release as state when there has not yet been a state change that was recorded.
    ///
    /// \param button the button to be checked
    ///
    /// \returns *Press* if \p button is pressed, *Release* if not
    [[nodiscard]] InputAction isMousePressed(MouseButton button) const;

    [[nodiscard]] std::size_t queuedEvents() const noexcept { return m_inputs.size(); }

private:
    std::queue<InputEvent> m_inputs;
    std::unordered_map<Key, InputAction> m_keyStates;
    std::unordered_map<MouseButton, InputAction> m_mouseStates;
    double m_mousePosX{ 0 };
    double m_mousePosY{ 0 };

    void processEvent(const KeyboardInputEvent& event);
    void processEvent(const MouseInputEvent& event);
    void processEvent(const MouseMoveEvent& event);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_INPUT_CONTROLLER_HPP

