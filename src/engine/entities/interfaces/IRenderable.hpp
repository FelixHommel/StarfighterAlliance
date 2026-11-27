#ifndef SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_RENDERABLE_HPP
#define SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_RENDERABLE_HPP

#include "rendering/IRenderSubmitter.hpp"

namespace sfa
{

/// Interface definition for Entities that can be rendered.
///
/// \author Felix Hommel
/// \date 11/16/2026
class IRenderable
{
public:
    IRenderable() = default;
    virtual ~IRenderable() = default;

    IRenderable(const IRenderable&) = default;
    IRenderable& operator=(const IRenderable&) = default;
    IRenderable(IRenderable&&) noexcept = default;
    IRenderable& operator=(IRenderable&&) noexcept = default;

    /// \brief Render the entity.
    ///
    /// \param renderer The \ref Renderer that is used to render the entity.
    virtual void render(IRenderSubmitter& renderer) const = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_RENDERABLE_HPP

