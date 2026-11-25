#ifndef SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP
#define SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP

#include "rendering/Renderer.hpp"
#include "utility/IWindow.hpp"

namespace sfa
{

// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members): This is a context struct that is just being passed to functions. it should npt own anything
struct RenderContext
{
    const IWindow& window;
    Renderer& renderer;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP

