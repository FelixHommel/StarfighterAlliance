#ifndef SFA_SRC_ENGINE_RENDERING_RENDERER_HPP
#define SFA_SRC_ENGINE_RENDERING_RENDERER_HPP

#include "rendering/RenderCommand.hpp"
#include "rendering/RenderExecutor.hpp"
#include "rendering/RenderQueue.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"

#include <memory>

namespace sfa
{

class Renderer
{
public:
    Renderer(std::unique_ptr<ISpritePipeline> spriteRenderer);
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;

    void beginFrame();
    void endFrame();

    void submit(RenderCommand command);

private:
    RenderQueue m_queue;
    RenderExecutor m_executor;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_RENDERER_HPP

