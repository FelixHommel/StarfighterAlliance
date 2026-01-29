#ifndef SFA_SRC_ENGINE_ECS_COMPONENT_MANAGER_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENT_MANAGER_HPP

#include "ComponentArray.hpp"
#include "ECSUtility.hpp"
#include "IComponentArray.hpp"
#include "components/IComponent.hpp"

#include <cassert>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utility>

namespace sfa
{

/// \brief Manage all components.
///
/// Maintains a \ref ComponentArray for every registered component, allowing entities to be added to the components.
///
/// \author Felix Hommel
/// \date 1/26/2026
class ComponentRegistry
{
public:
    ComponentRegistry() = default;
    ~ComponentRegistry() = default;

    ComponentRegistry(const ComponentRegistry&) = delete;
    ComponentRegistry& operator=(const ComponentRegistry&) = delete;
    ComponentRegistry(ComponentRegistry&&) = delete;
    ComponentRegistry& operator=(ComponentRegistry&&) = delete;

    /// \brief Register a new component.
    ///
    /// \tparam T Type of the component
    template<Component T>
    void registerComponent()
    {
        ComponentTypeID typeID{ getComponentTypeID<T>() };
#if defined(SFA_DEBUG)
        assert(!m_components.contains(typeID) && "Component already registered");
#endif // !SFA_DEBUG

        m_components[typeID] = std::make_unique<ComponentArray<T>>();
    }

    /// \brief Add the component to an entity.
    ///
    /// If the component wasn't registered before, the method will register the component and then add it to the entity.
    ///
    /// \tparam T Type of the component
    ///
    /// \param entity target entity
    /// \para component the component that is added to the entity
    template<Component T>
    void addComponent(EntityID entity, T component)
    {
        if(!isComponentRegistered<T>())
            registerComponent<T>();

        getComponentArray<T>().insert(entity, std::move(component));
    }

    /// \brief Remove a component from an enttiy.
    ///
    /// \tparam T Type of the component
    ///
    /// \param entity target entity
    template<Component T>
    void removeComponent(EntityID entity)
    {
#if defined(SFA_DEBUG)
        assert(isComponentRegistered<T>() && "Component is not registered");
        assert(contains<T>(entity) && "The entity does not have the component");
#endif // !SFA_DEBUG

        getComponentArray<T>().remove(entity);
    }

    /// \brief Get the component of an entity.
    ///
    /// \tparam T Type of the component
    ///
    /// \param entity the component of whom to get
    ///
    /// \returns reference to the component of \p entity
    template<Component T>
    T& getComponent(EntityID entity)
    {
#if defined(SFA_DEBUG)
        assert(isComponentRegistered<T>() && "Component is not registered");
        assert(contains<T>(entity) && "The entity does not have the component");
#endif // !SFA_DEBUG

        return getComponentArray<T>().get(entity);
    }

    /// \brief Get the component of an entity.
    ///
    /// \tparam T Type of the component
    ///
    /// \param entity the component of whom to get
    ///
    /// \returns const-ref to the component of \p entity
    template<Component T>
    const T& getComponent(EntityID entity) const
    {
#if defined(SFA_DEBUG)
        assert(isComponentRegistered<T>() && "Component is not registered");
        assert(contains<T>(entity) && "The entity does not have the component");
#endif // !SFA_DEBUG

        return getComponentArray<T>().get(entity);
    }

    /// \brief Check if an entity has the component.
    ///
    /// \tparam T Type of the component
    ///
    /// \param entity the entity to check
    ///
    /// \returns *true* if \p entity has the component, *false* otherwise
    template<Component T>
    bool contains(EntityID entity) const
    {
#if defined(SFA_DEBUG)
        assert(isComponentRegistered<T>() && "Component is not registered");
#endif // !SFA_DEBUG

        return getComponentArray<T>().contains(entity);
    }

    /// \brief Get the entire \ref ComponentArray of a component
    ///
    /// \tparam T Type of the component
    ///
    /// \returns the \ref ComponentArray of \p T
    template<Component T>
    ComponentArray<T>& getComponentArray()
    {
#if defined(SFA_DEBUG)
        assert(isComponentRegistered<T>() && "Component is not registered");
#endif // !SFA_DEBUG

        return static_cast<ComponentArray<T>&>(*m_components[getComponentTypeID<T>()]);
    }

    /// \brief Get the entire \ref ComponentArray of a component
    ///
    /// \tparam T Type of the component
    ///
    /// \returns the \ref ComponentArray of \p T
    template<Component T>
    const ComponentArray<T>& getComponentArray() const
    {
#if defined(SFA_DEBUG)
        assert(isComponentRegistered<T>() && "Component is not registered");
#endif // !SFA_DEBUG

        return static_cast<const ComponentArray<T>&>(*m_components.at(getComponentTypeID<T>()));
    }

    /// \brief Remove all components from an entity.
    ///
    /// \param entity target entity
    void entityDestroyed(EntityID entity)
    {
        for(auto& pair : m_components)
            pair.second->entityDestroyed(entity);
    }

    /// \brief Get the amount of registered components.
    ///
    /// \returns How many components have been registered
    [[nodiscard]] std::size_t registeredComponents() const noexcept { return m_components.size(); }

private:
    std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentArray>> m_components;

    /// \brief Check if a component is registered.
    ///
    /// \tparam T Type of the component
    ///
    /// \returns *true* if T has been registered, *false* otherwise
    template<typename T>
    bool isComponentRegistered() const
    {
        return m_components.contains(getComponentTypeID<T>());
    }
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENT_MANAGER_HPP

