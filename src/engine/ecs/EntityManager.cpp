#include "EntityManager.hpp"

#include "ecs/ECSUtility.hpp"

#include <cassert>

namespace sfa
{

EntityManager::EntityManager()
{
    for(EntityID entity{ 1 }; entity < MAX_ENTITIES; ++entity)
        m_availableEntities.push(entity);
}

EntityID EntityManager::createEntity()
{
#if defined(SFA_DEBUG)
    assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities");
#endif // !SFA_DEBUG

    EntityID id{ m_availableEntities.front() };
    m_availableEntities.pop();
    ++m_livingEntityCount;

    return id;
}

void EntityManager::destroyEntity(EntityID entity)
{
#if defined(SFA_DEBUG)
    assert(entity <= m_livingEntityCount && "Entity out of range");
#endif // !SFA_DEBUG

    m_availableEntities.push(entity);
    --m_livingEntityCount;
}

} // namespace sfa

