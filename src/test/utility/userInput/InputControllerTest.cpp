#include "utility/userInput/InputController.hpp"
#include "utility/userInput/InputEvent.hpp"

#include <catch2/catch_test_macros.hpp>
#include <glm/glm.hpp>

namespace sfa::testing
{

/// \brief Test the features of the \ref InputController.
///
/// \author Felix Hommel
/// \date 11/06/2026
TEST_CASE("InputControllerTest", "[unit][utility]")
{
    InputController controller{};

    /// \brief Test registering a new \ref KeyboardInputEvent.
    ///
    /// When a new \ref KeyboardInputEvent is registered, the internal event queue should be increased by one.
    SECTION("Register a new keyboard event")
    {
        controller.registerEvent(KeyboardInputEvent{ .key = Key::W, .action = InputAction::Press });

        CHECK(controller.queuedEvents() == 1);
    }

    /// \brief Test registering a new \ref MouseInputEvent.
    ///
    /// When a new \ref MouseInputEvent is registered, the internal event queue should be increased by one.
    SECTION("Register a new mouse event")
    {
        controller.registerEvent(MouseInputEvent{ .button = MouseButton::Left, .action = InputAction::Press });

        CHECK(controller.queuedEvents() == 1);
    }

    /// \brief Test registering a new \ref MouseMoveEvent.
    ///
    /// When a new \ref MouseMoveEvent is registered, the internal event queue should be increased by one.
    SECTION("Register a new mouse move event")
    {
        controller.registerEvent(MouseMoveEvent{ .posX = 1.f, .posY = 1.f });

        CHECK(controller.queuedEvents() == 1);
    }

    /// \brief Test if a key that was not pressed previously is pressed.
    ///
    /// When a key never has been pressed, the \ref InputController should interpret that key as being released.
    SECTION("Check key state of not previosuly pressed key")
    {
        CHECK(controller.isKeyPressed(Key::W) == InputAction::Release);
    }

    /// \brief Test if a key that was pressed is reported as pressed.
    ///
    /// When a key was pressed, the internals should record and save that state and then report the right result back.
    SECTION("Check the key state of a pressed key")
    {
        controller.registerEvent(KeyboardInputEvent{ .key = Key::W, .action = InputAction::Press });
        controller.processEventQueue();

        CHECK(controller.isKeyPressed(Key::W) == InputAction::Press);
    }

    /// \brief Test if a button that was not pressed previously is pressed.
    ///
    /// When a button never has been pressed, the \ref InputController should interpret that button as being released.
    SECTION("Check button state of not previously pressed key")
    {
        CHECK(controller.isMousePressed(MouseButton::Left) == InputAction::Release);
    }

    /// \brief Test if a button that was pressed is reported as pressed.
    ///
    /// When a button was pressed, the internals should record and save that state and then report the right result back.
    SECTION("Check button state of pressed key")
    {
        controller.registerEvent(MouseInputEvent{ .button = MouseButton::Left, .action = InputAction::Press });
        controller.processEventQueue();

        CHECK(controller.isMousePressed(MouseButton::Left) == InputAction::Press);
    }

    /// \brief Test the processing of \ref MouseMoveEvent.
    ///
    /// A \ref MouseMoveEvent should update the internal state of the \ref InputController so that it returns the new position
    /// of the mouse.
    SECTION("Check mouse movement event")
    {
        constexpr auto NEW_POSITION{ glm::dvec2(15.0, 51.0) };

        controller.registerEvent(MouseMoveEvent{ .posX = NEW_POSITION.x, .posY = NEW_POSITION.y });
        controller.processEventQueue();

        CHECK(controller.mousePosition() == NEW_POSITION);
    }

    /// \brief Test the behavior when a \ref KeyboardInputEvent with an unknown parameter is registered and processed.
    ///
    /// When the \ref KeyboardInputEvent contains any unknown parameters, the event should just be discarded.
    SECTION("Process unknown keyboard event")
    {
        controller.registerEvent(KeyboardInputEvent{ .key = Key::W, .action = InputAction::Unknown });
        controller.registerEvent(KeyboardInputEvent{ .key = Key::Unknown, .action = InputAction::Press });

        CHECK(controller.queuedEvents() == 2);

        controller.processEventQueue();

        CHECK(controller.isKeyPressed(Key::W) == InputAction::Release);
    }

    /// \brief Test the behavior when a \ref MouseInputEvent with an unknown parameter is registered and processed.
    ///
    /// When the \ref MouseInputEvent contains any unknown parameters, the event should just be discarded.
    SECTION("Process unknown mouse input event")
    {
        controller.registerEvent(MouseInputEvent{ .button = MouseButton::Left, .action = InputAction::Unknown });
        controller.registerEvent(MouseInputEvent{ .button = MouseButton::Unknown, .action = InputAction::Press });

        CHECK(controller.queuedEvents() == 2);

        controller.processEventQueue();

        CHECK(controller.isMousePressed(MouseButton::Left) == InputAction::Release);
    }
}

} // namespace sfa::testing

