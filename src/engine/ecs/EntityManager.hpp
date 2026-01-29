#ifndef SFA_SRC_ENGINE_ECS_ENTITY_MANAGER_HPP
#define SFA_SRC_ENGINE_ECS_ENTITY_MANAGER_HPP

#include "ECSUtility.hpp"

#include <cstddef>
#include <queue>

namespace sfa
{

/// \brief Create and destroy entities.
///
/// Manage a pool of entities and the availability of entities.
///
/// \author Felix Hommel
/// \date 1/26/2026
class EntityManager
{
public:
    EntityManager();
    ~EntityManager() = default;

    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;

    [[nodiscard]] std::size_t livingEntityCount() const noexcept { return m_livingEntityCount; }
    [[nodiscard]] static constexpr std::size_t maxEntities() noexcept { return MAX_ENTITIES; }

    /// \brief Create a new entity.
    ///
    /// \returns \ref EntityID of the new entity
    [[nodiscard]] EntityID createEntity();
    /// \brief Destroy an entity.
    /// 
    /// \param entity the entity that is being destroyed
    void destroyEntity(EntityID entity);

private:
    static constexpr std::size_t MAX_ENTITIES{ 10'000 };

    std::queue<EntityID> m_availableEntities;
    std::size_t m_livingEntityCount{ 0 };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_ENTITY_MANAGER_HPP

