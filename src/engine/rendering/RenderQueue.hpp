#ifndef SFA_SRC_ENGINE_RENDERING_RENDER_QUEUE_HPP
#define SFA_SRC_ENGINE_RENDERING_RENDER_QUEUE_HPP

#include "rendering/RenderCommand.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace sfa
{

/// \brief A queue used by the \ref Renderer to collect \ref RenderCommand during the submission phase.
///
/// \author Felix Hommel
/// \date 11/11/2026
class RenderQueue
{
public:
    RenderQueue() = default;
    ~RenderQueue() = default;

    RenderQueue(const RenderQueue&) = delete;
    RenderQueue& operator=(const RenderQueue&) = delete;
    RenderQueue(RenderQueue&&) noexcept = delete;
    RenderQueue& operator=(RenderQueue&&) noexcept = delete;

    [[nodiscard]] bool empty() const noexcept { return m_commands.empty(); }
    [[nodiscard]] std::size_t size() const noexcept { return m_commands.size(); }
    [[nodiscard]] std::span<const RenderCommand> commands() const noexcept { return m_commands; }

    /// \brief Submit a new \ref RenderCommand to the queue.
    ///
    /// \param cmd The \ref RenderCommand that is submitted
    void submit(RenderCommand cmd);
    /// \brief Clear the queue.
    void clear();

private:
    std::vector<RenderCommand> m_commands;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_RENDER_QUEUE_HPP

