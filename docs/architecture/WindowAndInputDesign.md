# Diagrams for the Window and User Input Abstraction

## Dependency Flowchart

The ```IWindow``` and ```IInputController``` system are closely related to each other, because in many windowing libraries
(i.e., GLFW) user input is coupled with window events. The engine should not be restricted by this coupling and should
not be dependent on a specific windowing library.

```mermaid
flowchart
  IInputController --> IWindow
  subgraph Window
    GLFWWindow --> IWindow
    GLFWWindow --> GLFW
  end
  subgraph UserInput
    InputController --> IInputController
  end
```

## Class Diagram

This class diagram shows the conceptual architecture using adapters to translate GLFW events to an engine native event
format, thereby directly eliminating close coupling of GLFW into the engine. All that while keeping the concerns as
much separated as possible.

```mermaid
classDiagram
  namespace InputEventSystem{
    class Key{
      <<enum>>
      W
      A
      S
      D
    }

    class MouseButton{
      <<enum>>
      RIGHT
      MIDDLE
      LEFT
    }

    class InputAction{
      <<enum>>
      RELEASE
      PRESS
    }

    class KeyboardInputEvent{
      +InputAction action
      +Key key
    }

    class MouseInputEvent{
      +InputAction action
      +MouseBtton button
    }

    class MouseMoveEvent{
      +double posX
      +double posY
    }

    class InputEvent
  }

  namespace InputSystem{
    class InputController{
      -array~bool, KEYS~ keysPressed
      -array~bool, MOUSE_BUTTONS~ buttonsPressed
      -queue~InputEvent~ events

      +drainEventQueue() void
    }

    class IInputController{
      +processEvent(InputEvent event)* void
      +isKeyPressed(Key key)* bool
      +isMousePressed(MouseButton button)* bool
    }
  }

  namespace WindowSystem{
    class IWindow{
      -IInputController controller

      +attachInputController(IInputController controller)* void
    }

    class GLFWWindow{
      -translateGLFWKeyEvent(int key, int action) KeyboardInputEvent
      -translateGLFWMouseMoveEvent(double posX, double posY) MouseMoveEvent
      -translateGLFWMouseButtonEvent(int button, int action) MouseInputEvent
    }
  }

  namespace ExternalLibraries{
    class GLFW
  }

  InputAction --> KeyboardInputEvent
  Key --> KeyboardInputEvent
  InputAction --> MouseInputEvent
  MouseButton --> MouseInputEvent

  KeyboardInputEvent --> InputEvent
  MouseInputEvent --> InputEvent
  MouseMoveEvent --> InputEvent

  IInputController ..|> InputController
  Key --> IInputController
  MouseButton --> IInputController
  InputEvent --> IInputController

  IWindow ..|> GLFWWindow
  GLFW ..> GLFWWindow
  KeyboardInputEvent --> GLFWWindow
  MouseInputEvent --> GLFWWindow
  MouseMoveEvent --> GLFWWindow

  IWindow <-- IInputController
```

## Implementation Details

The 3 types of event can be perfectly represented by a ```std::variant``` based approach. That way the
```IInputController::processEvent(const InputEvent& event)``` method can easily use ```st::visit``` to process all 3 types of
events.
Other than that, there is a consideration to make in the ```InputController``` class related to the queue-based
event approach:

1. Immediate State Progression
  The event is processed immediately when it is inserted into the queue.
2. Insert Queue + Process Separately
  Here, the events are inserted into the queue, and then processed once per game loop iteration.
