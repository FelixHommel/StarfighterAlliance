#ifndef SFA_SRC_ENGINE_RENDERING_I_RENDER_SUBMITTER_HPP
#define SFA_SRC_ENGINE_RENDERING_I_RENDER_SUBMITTER_HPP

#include "rendering/RenderCommand.hpp"

namespace sfa
{

/// \brief Interface definition for submitting renderables to a Renderer.
///
/// \author Felix Hommel
/// \date 11/27/2026
class IRenderSubmitter
{
public:
    IRenderSubmitter() = default;
    virtual ~IRenderSubmitter() = default;

    IRenderSubmitter(const IRenderSubmitter&) = delete;
    IRenderSubmitter& operator=(const IRenderSubmitter&) = delete;
    IRenderSubmitter(IRenderSubmitter&&) = delete;
    IRenderSubmitter& operator=(IRenderSubmitter&&) = delete;

    /// \brief Submit a new \ref RenderCommand to the Renderer.
    ///
    /// \param command The command that is submitted
    virtual void submit(RenderCommand command) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_I_RENDER_SUBMITTER_HPP

