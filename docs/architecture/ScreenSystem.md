# Architecture Decisions for the Screen System

> Felix Hommel, 3/2/2026

```mermaid
classDiagram
  namespace Screens{
    class MenuScreen
    class SpaceShipSelectionScreen
    class GameScreen
    class EndScreen

    class IScreen{
      <<Interface>>

      +onEnter()* void
      +onExit()* void

      +handleInput(InputController)* void
      +update(float dt)* void
      +render(RenderContext)* void

      +isOverlay() bool
    }

    class ScreenStack{
      -Array~IScreen~ stack
      -Queue~ScreenCommand~ pendingCommands

      +enqueueCommand(ScreenCommand) void
      +processCommands() void

      +handleInput(InputController) void
      +update(float dt) void
      +render(RenderContext) void

      -push(IScreen) void
      -pop() void
      -replace(IScreen) void
    }

    class ScreenCommandType{
      <<Enumeration>>

      Push
      Pop
      Replace
    }

    class ScreenCommand{
      ScreenCommandType type
      IScreen screen
    }

    class ScreenContext{
      <<Interface>>

      +push(IScreen)* void
      +pop()* void
      +replace(IScreen)* void
    }
  }

  namespace Core{
    class RenderContext{
      SpriteRenderer renderer
      TextRenderer textRenderer

      Viewport viewport
    }
  }

  namespace Utility{
    class InputController
  }

  namespace ECS{
    class ECSRegistry
    class Entity

    class UIButtonSystem
    class UIButton{
      -UICommand command
    }
    class UILayoutSystem
  }
  namespace ButtonUtility{
    class UICommand{
      <<Abstract>>

      +execute(ScreenContext) void
    }
    class StartGameCommand
    class QuitGameCommand
    class OpenScreenCommand
  }

  IScreen <|.. MenuScreen
  IScreen <|.. SpaceShipSelectionScreen
  IScreen <|.. GameScreen
  IScreen <|.. EndScreen

  ScreenStack "1" o-- "*" IScreen
  ScreenStack ..> RenderContext
  ScreenStack ..> InputController
  ScreenStack ..> ScreenCommand
  ScreenStack ..|> ScreenContext

  ScreenCommand --> ScreenCommandType

  ECSRegistry ..* IScreen
  ECSRegistry --> UIButton
  ECSRegistry --> UIButtonSystem
  ECSRegistry --> UILayoutSystem

  UICommand ..> ScreenContext
  UICommand <|.. StartGameCommand
  UICommand <|.. QuitGameCommand
  UICommand <|.. OpenScreenCommand

  UIButtonSystem ..> InputController

  UIButton --> UICommand
```

A top level orchestrator would then be able to manage the ```ScreenCommands``` to manipulate the ```ScreenStack```.

## Rendering Order

Given overlays should be on top of non-overlay screens, the queue has to traversed from top down to the first
non-overlay screen and then rendered from there to the top. This ensures that all overlays are over the main game screen
and in the proper order.

## Input Propagation

Compared to rendering order, input propagation should work exactly the other way around. Overlays usually should stop
events from being passed down the screen chain if the overlay can consume the event. If the overlay can not consume
the event, it can be passed down one layer further.
