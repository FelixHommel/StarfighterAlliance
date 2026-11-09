#include "EntityManager.hpp"

#include "ecs/ECSUtility.hpp"

#include <cassert>
#include <stdexcept>

namespace sfa
{

EntityManager::EntityManager()
{
    // NOTE: 0 represents the NULL_ENTITY -> strat valid ID range at 1
    for(EntityID entity{ 1 }; entity <= MAX_ENTITIES; ++entity)
        m_availableEntities.push(entity);
}

EntityID EntityManager::createEntity()
{
    if(m_livingEntityCount >= MAX_ENTITIES)
        throw std::runtime_error("Too many entities");

    EntityID id{ m_availableEntities.front() };
    m_availableEntities.pop();
    ++m_livingEntityCount;

    return id;
}

void EntityManager::destroyEntity(EntityID entity)
{
    if(entity > m_livingEntityCount)
        throw std::runtime_error("Entity out of range");

    m_availableEntities.push(entity);
    --m_livingEntityCount;
}

} // namespace sfa

