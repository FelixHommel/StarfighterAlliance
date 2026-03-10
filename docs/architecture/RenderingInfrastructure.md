# Concerns and Suggestions to Improve the Rendering Backend

Currently the Rendering is very intertwined with what we can render. We have a ```SpriteRenderer``` and
```TextRenderer```, both of which abstract away drawing behavior with a ```draw(...)``` function that requires numerous
arguments to be provided. Besides the fact that this approach carries the burden of state management, it also comes
with the added disadvantage of code repetition (i.e., ```beginFrame(...)``` method in both renderers).

## Overview of the Problem Infrastructure

```mermaid
classDiagram
  namespace Core{
    class Viewport
    class Shader

    class TextRenderer{
      -Shader textShader

      +TextRenderer(Shader)
      +beginFrame(Mat4x4) void
      +draw(String text, Vec2 pos, Vec2 scale, Vec3 color) void
    }

    class SpriteRenderer{
      -Shader spirteShader

      +SpriteRenderer(Shader)
      +beginFrame(Mat4x4) void
      +draw(Texture texture, Vec2 pos, Vec2 scale, float rotation, Vec3 color) void
    }
  }

  namespace ECS{
    class ComponentRegistry

    class SpriteRenderSystem{
        -SpriteRenderer spriteRenderer

        +SpriteRenderSystem(Shader spriteShader)
        +render(ComponentRegistry components, Mat4x4 projectionMatrix) void
    }

    class UIRenderSystem{
        -SpriteRenderer spriteRenderer
        -TextRenderer textRenderer

        +TextRenderer(Shader spriteShader, Shader textShader)
        +render(ComponentRegistry components, Viewport viewport) void
    }
  }

  namespace External{
    class OpenGL
    class FreeType
  }

  SpriteRenderer ..> OpenGL
  SpriteRenderer "*" o-- "1" Shader

  TextRenderer ..> OpenGL
  TextRenderer ..> FreeType
  TextRenderer "*" o-- "1" Shader

  UIRenderSystem ..> Viewport
  UIRenderSystem ..> ComponentRegistry
  UIRenderSystem "*" o-- "2" Shader
  UIRenderSystem "*" *-- "1" SpriteRenderer
  UIRenderSystem "*" *-- "1" TextRenderer

  SpriteRenderSystem ..> ComponentRegistry
  SpriteRenderSystem "*" o-- "1" Shader
  SpriteRenderSystem "*" *-- "1" SpriteRenderer
```

There are a few things in this entire system that makes sense. For example, it makes sense that something that renders
text text using FreeType needs access to FreeType, and things that need Rendering need access to OpenGL. What is
suboptimal, is that they are not labeled with the things they are using and that there is no common interface to use
the services they provide. What that means is that everyone that uses Rendering is directly depending on OpenGL and FreeType.

## Proposal: Implement a Strategy-Pattern Based Solution (Per Drawable)

When using a strategy based solution, it would be possible to abandon the rigid rendering structure and replace them
with simple drawing primitives. These drawing primitives are then responsible for providing the rendering strategy with
what is needed to draw the primitive (This means i.e., a sprite provides a texture, rotation, position, ... to the strategy).

```mermaid
classDiagram
  namespace Core_Rendering{
    class IDrawingPrimitive{
      <<Interface>>

      +draw() void
    }

    class Sprite{
      -Vec4 color
      -Texture texture

      +draw() void
    }

    class Text{
      -String text
      -Float size

      +draw() void
    }

    class IDrawingStrategy~T~{
      <<Interface>>

      +draw(T)* void
    }

    class OpenGLSpriteStrategy{
      +OpenGLSpriteStrategy(Shader)

      +draw(Sprite) void
    }

    class FreeTypeTextStrategy{
      +FreeTypeTextStrategy(Shader)

      +draw(Text) void
    }
  }

  OpenGLSpriteStrategy ..> Sprite
  OpenGLSpriteStrategy --|> IDrawingStrategy

  IDrawingStrategy <--* IDrawingPrimitive

  IDrawingPrimitive <|-- Sprite
  IDrawingPrimitive <|-- Text

  FreeTypeTextStrategy --|> IDrawingStrategy
  FreeTypeTextStrategy ..> Text
```

This system then could be implemented like this (semi-implementation, just for conceptual idea):

```cpp
class IDrawingPrimitive
{
public:
    IDrawingPrimitive() = default;
    virtual ~IDrawingPrimitive() = default;

    virtual void draw() const = 0;
};

class Sprite : public IDrawingPrimitive
{
public:
    explicit Sprite(glm::vec4 color, std::shared_ptr<DrawStrategy<Circle>> strat)
        : m_strategy{ std::move(strat)
        , m_color{ color }
    {}

    explicit Sprite(glm::vec4 color, std::shared_ptr<Shader> shader)
        : m_strategy{ std::make_unique<OpenGLSpriteStrategy>(std::move(shader)) }
        , m_color{ color }
    {}

    void draw() const override { m_strategy->draw(*this); }

private:
    std::unique_ptr<DrawStrategy<Sprite>> m_strategy;
    glm::vec4 m_color;
};

template<typename T>
class DrawStrategy
{
public:
    virtual ~DrawStrategy() = default;

    virtual void draw(const T&, /* potential other arguments */) const = 0;
};

class OpenGLSpriteStrategy : public DrawStrategy<Sprite>
{
public:
    OpenGLSpriteStrategy(std::shared_ptr<Shader> shader, /* drawing related arguments */);

    void draw(const Sprite& s, /* potential other arguments */) const override;

private:
    std::shared_ptr<Shader> m_shader;
};
```

This system can be combined with free-functions that are used to setup the frame global state
(i.e., setting projection matrix, transparency, stencil, ...) with functions like ```void beginOpenGLFrame(...)```

That means that every primitive carries its own way of rendering with it as a data member while keeping the benefit of being lightweight.

### Critical Considerations

- Examine the overhead of creating an OpenGL buffer per sprite. That would be an implied restriction, when every sprite
  has its own strategy, that would mean that the strategy would need to handle creating an OpenGL buffer for
  the primitive. That is because every Sprite needs a quad to project the texture or color onto.

  - Same problem with the Text primitive, possibly even worse if each strategy would need to load its own font and
    create glyphs from that font

- How can every used shader know about the current projection matrix when setting the frame global data?

## Proposal: Implement a Command + Strategy Based Solution

This approach would avoid the issues of the prior solution which had a strategy per drawable, that would've caused a
violation of the SRP because a Sprite then would have been responsible for data maintenance and rendering.

Here, render primitives become pure data containers that do not manage anything else than maintaining the data they contain.

```mermaid
classDiagram
  namespace Core_Primitives{
    class Mesh

    class Text{
      +String content
      +Vec2 position
      +Vec2 scale
      +Vec4 color
      +FreeTypeFont font
    }

    class Sprite{
      +Texture texture
      +Vec2 position
      +Vec2 scale
      +Float rotation
      +Vec4 color
    }
  }

  namespace Core_Rendering{
    class Glyph{
      +Uint texture
      +IVec2 size
      +IVec2 bearing
      +Uint advance
    }

    class FreeTypeFont{
      -Map~Char, Glyph~ glyphs

      +loadFont(Path filepath) void
      +glyph(Char) Glyph
    }

    class OpenGLTextRenderer{
      -UInt vao
      -UInt vbo

      +draw(Text) void
    }

    class OpenGLRenderBackend{
      -OpenGLTextRenderer textRenderer

      +beginFrame(FrameData) void
      +draw(Sprite) void
      +draw(Text) void
      +draw(Mesh) void
    }

    class IRenderBackend{
      <<Interface>>

      +beginFrame(FrameData)* void
      +draw(Sprite)* void
      +draw(Text)* void
      +draw(Mesh)* void
    }

    class FrameData{
      +Mat4x4 projection
      +Float dt
    }
  }

  Glyph ..* FreeTypeFont

  IRenderBackend <.. FrameData
  IRenderBackend <|.. OpenGLRenderBackend

  OpenGLRenderBackend *.. OpenGLTextRenderer

  OpenGLTextRenderer ..> FreeTypeFont
```
