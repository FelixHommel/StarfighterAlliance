#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_UI_RENDER_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_UI_RENDER_SYSTEM_HPP

#include "core/SpriteRenderer.hpp"
#include "core/TextRenderer.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "utility/GLFWWindow.hpp"

#include <memory>

namespace sfa
{

class UIRenderSystem
{
public:
    UIRenderSystem(std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<TextRenderer> textRenderer);

    void render(ComponentRegistry& registry, const Viewport& viewport);

private:
    std::shared_ptr<SpriteRenderer> m_spriteRenderer;
    std::shared_ptr<TextRenderer> m_textRenderer;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_UI_RENDER_SYSTEM_HPP

