#ifndef SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_UPDATEABLE_HPP
#define SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_UPDATEABLE_HPP

namespace sfa
{

/// Interface definition for entities that have some sort of updateable state.
///
/// \author Felix Hommel
/// \date 11/16/2026
class IUpdateable
{
public:
    IUpdateable() = default;
    virtual ~IUpdateable() = default;

    IUpdateable(const IUpdateable&) = default;
    IUpdateable& operator=(const IUpdateable&) = default;
    IUpdateable(IUpdateable&&) noexcept = default;
    IUpdateable& operator=(IUpdateable&&) noexcept = default;

    /// \brief Update the state of the entity.
    ///
    /// \param dt The timestep
    virtual void update(float dt) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_UPDATEABLE_HPP

