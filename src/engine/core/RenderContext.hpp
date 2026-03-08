#ifndef SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP
#define SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP

namespace sfa
{

struct RenderContext
{
    RenderContext() = default;
    ~RenderContext() = default;

    RenderContext(const RenderContext&) = default;
    RenderContext& operator=(const RenderContext&) = default;
    RenderContext(RenderContext&&) = default;
    RenderContext& operator=(RenderContext&&) = default;

    // FIXME: add the members to the RenderContext
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP

