# Architecture Decisions for Resource Loading

> Felix Hommel, 2/28/2026

The resource loading and management system is designed to be working in parallel. That means resource loading is a
multithreading task. This can be achieved by separating resource loading in two separate stages:

1. Load the raw data from disk
2. Upload OpenGL resource to the GPU

Step 1 is the part that can benefit from multithreading since every resource can be loaded by a separate thread which
then can save the raw data (i.e., shader source code, image pixel data). In step 2, the main thread then can create
OpenGL resources from the raw data and upload the relevant parts to the GPU. The upload process needs to be done by the
thread that creates the OpenGL context, the main thread, because of the way OpenGL handles its context.

```mermaid
classDiagram
  namespace ResourceLoading{
    class ShaderSourceData{
      +String vertexSource
      +String fragmentSource
      +String geometrySource
    }

    class TextureRawData{
      +Int width
      +Int height
      +Int channels
      +Array~Bytes~ pixels
    }

    class ResourceError{
    }

    class ShaderLoadRequest{
      +String name
      +Path vert
      +Path frag
      +Path geom
    }

    class TextureLoadRequest{
      +String name
      +Path filepath
    }

    class ResourceContext{
      +ResourceContext(IResourceLoader loader)

      +requestResource(Variant~ShaderLoadRequest, TextureLoadRequest~ request) void

      +getShader(String key) Shader
      +getTexture(String key) Texture
    }
    class ResourceLoader{
      +loadShader(Path vert, Path frag, Path geom = "") LoadResult
      +loadTexture(Path filepath) LoadResult

      -readFile(Path filepath) Expected~String, ResourceError~
    }
    class IResourceLoader{
      <<Interface>>

      +loadShader(Path vert, Path frag, Path geom = "")* LoadResult
      +loadTexture(Path filepath)* LoadResult
    }
    class ResourceCache~T~{
      -Map~String, T~ resources

      +store(String key, T resource) void
      +get(String key) void
      +contains(String key) bool
    }
  }
```
