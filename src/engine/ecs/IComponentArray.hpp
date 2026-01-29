#ifndef SFA_SRC_ENGINE_ECS_I_COMPONENT_ARRAY_HPP
#define SFA_SRC_ENGINE_ECS_I_COMPONENT_ARRAY_HPP

#include "ECSUtility.hpp"

namespace sfa
{

/// \brief Interface for ComponentArray to make type-erased storage possible.
///
/// \author Felix Hommel
/// \date 1/25/2026
class IComponentArray
{
public:
    IComponentArray() = default;
    virtual ~IComponentArray() = default;

    IComponentArray(const IComponentArray&) = delete;
    IComponentArray& operator=(const IComponentArray&) = delete;
    IComponentArray(IComponentArray&&) = delete;
    IComponentArray& operator=(IComponentArray&&) = delete;

    /// \brief If the entity has the component then remove it.
    ///
    /// \param entity the target entity
    virtual void entityDestroyed(EntityID entity) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_I_COMPONENT_ARRAY_HPP

