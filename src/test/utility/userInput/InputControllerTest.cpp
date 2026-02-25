#include "utility/userInput/InputController.hpp"
#include "utility/userInput/InputEvent.hpp"

#include <gtest/gtest.h>

#include <memory>

namespace sfa::testing
{

/// \brief Test the features of the \ref InputController.
///
/// \author Felix Hommel
/// \date 2/25/2026
class InputControllerTest : public ::testing::Test
{
public:
    InputControllerTest() = default;
    ~InputControllerTest() override = default;

    InputControllerTest(const InputControllerTest&) = delete;
    InputControllerTest& operator=(const InputControllerTest&) = delete;
    InputControllerTest(InputControllerTest&&) noexcept = delete;
    InputControllerTest& operator=(InputControllerTest&&) noexcept = delete;

protected:
    std::unique_ptr<InputController> m_controller{ std::make_unique<InputController>() };
};

/// \brief Test registering a new \ref KeyboardInputEvent.
///
/// When a new \ref KeyboardInputEvent is registered, the internal event queue should be increased by one.
TEST_F(InputControllerTest, RegisterNewKeyboardEvent)
{
    m_controller->registerEvent(KeyboardInputEvent{ .key = Key::W, .action = InputAction::Press });

    EXPECT_EQ(1, m_controller->queuedEvents());
}

/// \brief Test registering a new \ref MouseInputEvent.
///
/// When a new \ref MouseInputEvent is registered, the internal event queue should be increased by one.
TEST_F(InputControllerTest, RegisterNewMouseEvent)
{
    m_controller->registerEvent(MouseInputEvent{ .button = MouseButton::Left, .action = InputAction::Press });

    EXPECT_EQ(1, m_controller->queuedEvents());
}

/// \brief Test registering a new \ref MouseMoveEvent.
///
/// When a new \ref MouseMoveEvent is registered, the internal event queue should be increased by one.
TEST_F(InputControllerTest, RegisterNewMouseMoveEvent)
{
    m_controller->registerEvent(MouseMoveEvent{ .posX = 1.f, .posY = 1.f});

    EXPECT_EQ(1, m_controller->queuedEvents());
}

/// \brief Test if a key that was not pressed previously is pressed.
///
/// When a key never has been pressed, the \ref InputController should interpret that key as being released.
TEST_F(InputControllerTest, CheckKeyStateOfNotPreviouslyPressedKey)
{
    EXPECT_EQ(InputAction::Release, m_controller->isKeyPressed(Key::W));
}

/// \brief Test if a key that was pressed is reported as pressed.
///
/// When a key was pressed, the internals should record and save that state and then report the right result back.
TEST_F(InputControllerTest, CheckKeyStateOfPressedKey)
{
    m_controller->registerEvent(KeyboardInputEvent{ .key = Key::W, .action = InputAction::Press });
    m_controller->processEventQueue();

    EXPECT_EQ(InputAction::Press, m_controller->isKeyPressed(Key::W));
}

/// \brief Test if a button that was not pressed previously is pressed.
///
/// When a button never has been pressed, the \ref InputController should interpret that button as being released.
TEST_F(InputControllerTest, CheckButtonStateOfNotPreviouslyPressedKey)
{
    EXPECT_EQ(InputAction::Release, m_controller->isMousePressed(MouseButton::Left));
}

/// \brief Test if a button that was pressed is reported as pressed.
///
/// When a button was pressed, the internals should record and save that state and then report the right result back.
TEST_F(InputControllerTest, CheckButtonStateOfPressedKey)
{
    m_controller->registerEvent(MouseInputEvent{ .button = MouseButton::Left, .action = InputAction::Press });
    m_controller->processEventQueue();

    EXPECT_EQ(InputAction::Press, m_controller->isMousePressed(MouseButton::Left));
}

/// \brief Test the processing of \ref MouseMoveEvent.
///
/// A \ref MouseMoveEvent should update the internal state of the \ref InputController so that it returns the new position
/// of the mouse.
TEST_F(InputControllerTest, CheckMouseMovementEvent)
{
    constexpr MousePosition newPosition{ .posX = 15.0, .posY = 51.0 };

    m_controller->registerEvent(MouseMoveEvent{ .posX = newPosition.posX, .posY = newPosition.posY });
    m_controller->processEventQueue();

    EXPECT_EQ(newPosition, m_controller->mousePosition());
}

/// \brief Test the behavior when a \ref KeyboardInputEvent with an unknown parameter is registered and processed.
///
/// When the \ref KeyboardInputEvent contains any unknown parameters, the event should just be discarded.
TEST_F(InputControllerTest, ProcessUnknownKeyboardEvent)
{
    m_controller->registerEvent(KeyboardInputEvent{ .key = Key::W, .action = InputAction::Unknown });
    m_controller->registerEvent(KeyboardInputEvent{ .key = Key::Unknown, .action = InputAction::Press });

    EXPECT_EQ(2, m_controller->queuedEvents());

    m_controller->processEventQueue();

    EXPECT_EQ(InputAction::Release, m_controller->isKeyPressed(Key::W));
}

/// \brief Test the behavior when a \ref MouseInputEvent with an unknown parameter is registered and processed.
///
/// When the \ref MouseInputEvent contains any unknown parameters, the event should just be discarded.
TEST_F(InputControllerTest, ProcessUnknownMouseInputEvent)
{
    m_controller->registerEvent(MouseInputEvent{ .button = MouseButton::Left, .action = InputAction::Unknown });
    m_controller->registerEvent(MouseInputEvent{ .button = MouseButton::Unknown, .action = InputAction::Press });

    EXPECT_EQ(2, m_controller->queuedEvents());

    m_controller->processEventQueue();

    EXPECT_EQ(InputAction::Release, m_controller->isMousePressed(MouseButton::Left));
}

} // namespace sf::testing

