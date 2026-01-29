#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_MOVEMENT_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_MOVEMENT_SYSTEM_HPP

#include "ecs/ComponentRegistry.hpp"

namespace sfa
{

/// \brief The \ref MovementSystem is responsible to handle movement for all entities that can move.
///
/// In order for an entity to be able to move, it needs to have a \ref TransformComponent as well as a \ref VelocityComponent.
///
/// \author Felix Hommel
/// \date 1/26/2026
class MovementSystem
{
public:
    MovementSystem() = default;
    ~MovementSystem() = default;

    MovementSystem(const MovementSystem&) = delete;
    MovementSystem& operator=(const MovementSystem&) = delete;
    MovementSystem(MovementSystem&&) = delete;
    MovementSystem& operator=(MovementSystem&&) = delete;

    /// \brief Move capable entities.
    ///
    /// An entity is capable if the entity has a \ref TransformComponent and a \ref VelocityComponent.
    ///
    /// \param components reference to \ref ComponentRegistry, which maintains the components
    /// \param dt delta time
    static void update(ComponentRegistry& components, float dt);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_MOVEMENT_SYSTEM_HPP

