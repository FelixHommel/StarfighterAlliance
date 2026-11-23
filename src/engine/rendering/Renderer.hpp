#ifndef SFA_SRC_ENGINE_RENDERING_RENDERER_HPP
#define SFA_SRC_ENGINE_RENDERING_RENDERER_HPP

#include "rendering/RenderCommand.hpp"
#include "rendering/RenderExecutor.hpp"
#include "rendering/RenderQueue.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace sfa
{

class Renderer
{
public:
    Renderer(std::unique_ptr<ISpritePipeline> spritePipeline, std::unique_ptr<ITextPipeline> textPipeline);
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;

    void beginFrame(const glm::mat4& projection);
    void endFrame();

    void submit(RenderCommand command);

private:
    std::unique_ptr<ISpritePipeline> m_spritePipeline;
    std::unique_ptr<ITextPipeline> m_textPipeline;

    RenderQueue m_queue;
    RenderExecutor m_executor;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_RENDERER_HPP

