#ifndef SFA_SRC_ENGINE_UTILITY_USER_INPUT_INPUT_EVENT_HPP
#define SFA_SRC_ENGINE_UTILITY_USER_INPUT_INPUT_EVENT_HPP

#include <cstdint>
#include <variant>

namespace sfa
{

enum class Key : std::uint8_t
{
    W,
    A,
    S,
    D,
    Esc,
    Unknown
};

enum class MouseButton : std::uint8_t
{
    Right,
    Middle,
    Left,
    Unknown
};

enum class InputAction : std::uint8_t
{
    Release,
    Press,
    Unknown
};

struct KeyboardInputEvent
{
    Key key;
    InputAction action;
};

struct MouseInputEvent
{
    MouseButton button;
    InputAction action;
};

struct MouseMoveEvent
{
    double posX;
    double posY;
};

using InputEvent = std::variant<KeyboardInputEvent, MouseInputEvent, MouseMoveEvent>;

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_USER_INPUT_INPUT_EVENT_HPP

