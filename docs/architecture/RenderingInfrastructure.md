# StarfighterAlliance - Rendering

## Problem

The problem is the current rendering implementation. There is a renderer for each specific renderable, which while
being functional is not really convenient to use neither very maintainable. 

## Solution

A better approach would be to implement rendering using a render queue system. The render queue system would not only
make it more convenient to interact with the rendering API but also would allow for easier addition of new renderables.
In the long term it also provides a better chance for extensibility and performance scaling because of techniques like
batching and render passes.

## Architecture

```mermaid
classDiagram
  class SpriteSubmission{
    +optional~TextureView~ texture
    +shared_ptr~Shader~ shader
    +vec2 size
    +vec3 color
    +vec2 position
    +vec2 scale
    +float rotation
  }

  class TextSubmission{
    +string text
    +vec4 color
    +vec2 position
    +vec2 scale
  }

  class RenderCommand{
    <<variant>>
    SpriteSubmission
    TextSubmission
  }

  class ISpritePipeline{
    +draw(submission: SpriteSubmission) void*
  }

  class BasicSpritePipeline{
    +draw(submission: SpriteSubmission) void
  }

  class RenderQueue{
    -vector~RenderCommand~ commands

    +commands() span~const RenderCommand~
    +submit(command: RenderCommand) void
    +clear() void
  }

  class RenderExecutor{
    -unique_ptr~ISpritePipeline~ spritePipeline

    +execute(commands: span~const RenderCommand~) void

    -executeImpl(submission: SpriteSubmission) void
    -executeImpl(submission: TextSubmission) void
  }

  class Renderer{
    -RenderQueue queue
    -RenderExecutor executor

    +beginFrame() void
    +endFrame() void
    +submit(command: RenderCommand) void
  }

  Renderer "1" *-- "1" RenderQueue
  Renderer "1" *-- "1" RenderExecutor
  Renderer ..> RenderCommand : submits

  RenderQueue "1" *-- "*" RenderCommand : contains

  RenderExecutor "1" *-- "1" ISpritePipeline
  RenderExecutor ..> RenderCommand : executes

  ISpritePipeline ..> SpriteSubmission : processes

  BasicSpritePipeline --|> ISpritePipeline

  RenderCommand ..> SpriteSubmission
  RenderCommand ..> TextSubmission
```
