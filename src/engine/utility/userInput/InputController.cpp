#include "InputController.hpp"

#include "utility/userInput/InputEvent.hpp"

#include <spdlog/spdlog.h>

#include <variant>

namespace sfa
{

void InputController::processEventQueue()
{
    while(!m_inputs.empty())
    {
        std::visit([this](const auto& event) { this->processEvent(event); }, m_inputs.front());

        m_inputs.pop();
    }
}

InputAction InputController::isKeyPressed(Key key) const
{
    if(!m_keyStates.contains(key))
        return InputAction::Release;

    return m_keyStates.at(key);
}

InputAction InputController::isMousePressed(MouseButton button) const
{
    if(!m_mouseStates.contains(button))
        return InputAction::Release;

    return m_mouseStates.at(button);
}

/// \brief Process a \ref KeyboardInputEvent.
///
/// If either the key or the action is unknown, the event is discarded.
///
/// \param event \ref KeyboardInputEvent containing the state change of the keyboard
void InputController::processEvent(const KeyboardInputEvent& event)
{
    if(event.key == Key::Unknown || event.action == InputAction::Unknown)
    {
        spdlog::warn("Keyboard Input Event with unknown parameters processed");
        return;
    }

    m_keyStates.insert_or_assign(event.key, event.action);
}

/// \brief Process a \ref MouseInputEvent.
///
/// If either the button or the action is unknown, the event is discarded.
///
/// \param event \ref MouseInputEvent containing the state change of the mouse
void InputController::processEvent(const MouseInputEvent& event)
{
    if(event.button == MouseButton::Unknown || event.action == InputAction::Unknown)
    {
        spdlog::warn("Mouse Input Event with unknown parameters processed");
        return;
    }

    m_mouseStates.insert_or_assign(event.button, event.action);
}

/// \brief Process a \ref MouseMoveEvent.
///
/// \param event \ref MouseMoveEvent containing the state change of the mouse position
void InputController::processEvent(const MouseMoveEvent& event)
{
    m_mousePosX = event.posX;
    m_mousePosY = event.posY;
}

}

