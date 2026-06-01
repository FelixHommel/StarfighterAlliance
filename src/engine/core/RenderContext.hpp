#ifndef SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP
#define SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP

#include "core/SpriteRenderer.hpp"
#include "ecs/systems/UIRenderSystem.hpp"
#include "utility/IWindow.hpp"

namespace sfa
{

// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members): This is a context struct that is just being passed to functions. it should npt own anything
struct RenderContext
{
    SpriteRenderer* spriteRenderer;
    UIRenderSystem& uiRenderer;
    const IWindow& window;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RENDER_CONTEXT_HPP

