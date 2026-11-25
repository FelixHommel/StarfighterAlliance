#ifndef SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_COLLDIABLE_HPP
#define SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_COLLDIABLE_HPP

#include "physics/Shape.hpp"

namespace sfa
{

/// Interface definition for entities that have collidable physics.
///
/// \author Felix Hommel
/// \date 11/16/2026
class ICollidable
{
public:
    ICollidable() = default;
    virtual ~ICollidable() = default;

    ICollidable(const ICollidable&) = default;
    ICollidable& operator=(const ICollidable&) = default;
    ICollidable(ICollidable&&) = default;
    ICollidable& operator=(ICollidable&&) = default;

    /// \brief Access the bounds
    [[nodiscard]] virtual physics::Shape bounds() const noexcept = 0;

    /// \brief Check if the entity collides another entity.
    ///
    /// \param other the \ref ICollidable.
    virtual void onCollision(const ICollidable& other) const = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ENTITIES_INTERFACES_I_COLLDIABLE_HPP

