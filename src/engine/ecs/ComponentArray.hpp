#ifndef SFA_SRC_ENGINE_ECS_COMPONENT_ARRAY_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENT_ARRAY_HPP

#include "ECSUtility.hpp"
#include "IComponentArray.hpp"
#include "components/IComponent.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <span>
#include <unordered_map>

namespace sfa
{

/// \brief Array to group components.
///
/// Manages a dense array of components and maps between components and which entity they belong to.
///
/// \tparam Type of the component.
///
/// \author Felux Hommel
/// \date 1/25/2026
template<Component T>
class ComponentArray : public IComponentArray
{
public:
    ComponentArray() = default;
    ~ComponentArray() override = default;

    ComponentArray(const ComponentArray&) = delete;
    ComponentArray(ComponentArray&&) = delete;
    ComponentArray& operator=(const ComponentArray&) = delete;
    ComponentArray& operator=(ComponentArray&&) = delete;

    /// \brief Add a component to an entity.
    ///
    /// \param entity the target entity
    /// \param component the new component
    void insert(EntityID entity, T component)
    {
#if defined(SFA_DEBUG)
        assert(!m_entityToIndex.contains(entity) && "Component already exists");
#endif // !SFA_DEBUG

        const std::size_t newIndex{ m_actualSize++ };
        m_entityToIndex[entity] = newIndex;
        m_indexToEntity[newIndex] = entity;
        m_components[newIndex] = component;
    }

    /// \brief Remove the component from an entity.
    ///
    /// \param entity the target entity
    void remove(EntityID entity)
    {
#if defined(SFA_DEBUG)
        assert(m_entityToIndex.contains(entity) && "Component doesn't exist");
#endif // !SFA_DEBUG

        // NOTE: Swap with last element to maintain density
        const std::size_t indexOfRemoved{ m_entityToIndex[entity] };
        const std::size_t indexOfLast{ m_actualSize - 1 };
        m_components[indexOfRemoved] = m_components[indexOfLast];

        // NOTE: Update mappings
        const EntityID entityOfLast{ m_indexToEntity[indexOfLast] };
        m_entityToIndex[entityOfLast] = indexOfRemoved;
        m_indexToEntity[indexOfRemoved] = entityOfLast;

        m_entityToIndex.erase(entity);
        m_indexToEntity.erase(indexOfLast);
        --m_actualSize;
    }

    /// \brief Get the component of an entity.
    ///
    /// \param entity the target entity
    ///
    /// \returns reference to component of \p entity
    T& get(EntityID entity)
    {
#if defined(SFA_DEBUG)
        assert(m_entityToIndex.contains(entity) && "Component doesn't exist");
#endif // !SFA_DEBUG

        return m_components[m_entityToIndex[entity]];
    }

    /// \brief Get the component of an entity.
    ///
    /// \param entity the target entity
    ///
    /// \returns const-ref to component of \p entity
    const T& get(EntityID entity) const
    {
#if defined(SFA_DEBUG)
        assert(m_entityToIndex.contains(entity) && "Component doesn't exist");
#endif // !SFA_DEBUG

        return m_components.at(m_entityToIndex.at(entity));
    }

    /// \brief Check if an entity has the component.
    ///
    /// \param entity the target entity
    ///
    /// \returns *true* if the entity has the component, *false* otherwise
    bool contains(EntityID entity) const { return m_entityToIndex.contains(entity); }

    /// \brief Get the entity ID of a component.
    ///
    /// \param index the index of the component
    ///
    /// \returns \ref EntityID that owns the component at \p index
    EntityID entityAtIndex(std::size_t index) const
    {
#if defined(SFA_DEBUG)
        assert(index <= m_actualSize && "Index is out of range");
#endif // !SFA_DEBUG

        return m_indexToEntity.at(index);
    }

    /// \brief Destroy the component of an entity if the component exists.
    ///
    /// \param entity the entity of which the component is getting destroyed
    void entityDestroyed(EntityID entity) override
    {
        if(m_entityToIndex.contains(entity))
            remove(entity);
    }

    std::size_t size() const noexcept { return m_actualSize; }

    auto begin() noexcept { return m_components.begin(); };
    auto cbegin() const noexcept { return m_components.cbegin(); }
    auto end() noexcept { return m_components.begin() + static_cast<std::ptrdiff_t>(m_actualSize); }
    auto cend() const noexcept { return m_components.cbegin() + static_cast<std::ptrdiff_t>(m_actualSize); }

    std::span<T> span() noexcept { return { m_components.data(), m_actualSize }; }
    std::span<const T> span() const noexcept { return { m_components.data(), m_actualSize }; }

private:
    static constexpr std::size_t MAX_COMPONENTS{ 10000 };

    std::array<T, MAX_COMPONENTS> m_components;
    std::unordered_map<EntityID, std::size_t> m_entityToIndex;
    std::unordered_map<std::size_t, EntityID> m_indexToEntity;
    std::size_t m_actualSize{ 0 };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENT_ARRAY_HPP

