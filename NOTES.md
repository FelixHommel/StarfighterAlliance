# NOTES

## Improvement Opportunities

### RenderContext


`RenderContext` could easily become a dumping ground of shared state, which is to be avoided. A thinkable better
alternative would be something that makes resource access consistent. This could be achieved through things like a
`EngineServices` or `IResourceProvider` feature. That way a god struct is avoided and more clarity is created.

### Open-Closed Principle in Rendering Infrastructure

When adding a new renderable type (i.e., particles), it is necessary to modify `RenderCommand`, `RenderExecutor`, and
`Renderer`. This should be modified to offer more flexibility for future type additions. Something like a
`std::tuple<IRenderPipeline<Ts>&...>` is thinkable to make it easier on users.

