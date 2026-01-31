#include "ecs/EntityManager.hpp"

#include <gtest/gtest.h>

#include <cstddef>

namespace sfa
{

/// \brief Test the \ref EntityManager class.
///
/// \author Felix Hommel
/// \date 1/27/2026
class EntityManagerTest : public ::testing::Test
{
public:
    EntityManagerTest() = default;
    ~EntityManagerTest() override = default;

    EntityManagerTest(const EntityManagerTest&) = delete;
    EntityManagerTest& operator=(const EntityManagerTest&) = delete;
    EntityManagerTest(EntityManagerTest&&) = delete;
    EntityManagerTest& operator=(EntityManagerTest&&) = delete;
};

using EntityManagerDeathTest = EntityManagerTest;

/// \brief Test the Construction of a \ref EntityManager.
///
/// After the \re EntityManager is constructed, it should not have allocated any entities yet.
TEST_F(EntityManagerTest, Construction)
{
    EntityManager manager{};

    EXPECT_EQ(manager.livingEntityCount(), 0);
}

/// \brief Test the creation of new entities in the \ref EntityManager.
///
/// When an entity is created, the living entity count is increased. Entity IDs are ascending.
TEST_F(EntityManagerTest, CreateEntity)
{
    EntityManager manager{};

    const auto id{ manager.createEntity() };

    EXPECT_EQ(id, 1);
    EXPECT_EQ(manager.livingEntityCount(), 1);
}

/// \brief Test the behavior when the \ref EntityManager has already created its maximum of entities.
///
/// When the maximum of entities already is created, the createEntity() method should fail an assertion when build in
/// debug mode.
TEST_F(EntityManagerDeathTest, CreateEntityExceedsLimit)
{
    EntityManager manager{};

    for(std::size_t i{ 0 }; i < EntityManager::maxEntities(); ++i)
        auto x{ manager.createEntity() };

    EXPECT_DEATH({ auto x{ manager.createEntity() }; }, "Too many entities");
}

/// \brief Test the behavior of the destroyEntity() method on a before created entity.
///
/// When an entity that was previously created is destroyed, the living entity count should be decreased by one.
TEST_F(EntityManagerTest, DestroyEntities)
{
    EntityManager manager{};
    const auto entity{ manager.createEntity() };

    manager.destroyEntity(entity);

    EXPECT_EQ(manager.livingEntityCount(), 0);
}

/// \brief Test the behavior of the destroyEntity() method when attempting to destroy an entity that never was created.
///
/// When an entity that never was created is attempted to be deleted, the destroyEntity() method should fail an
/// assertion when build in debug mode.
TEST_F(EntityManagerDeathTest, DestroyNonExistingEntitie)
{
    EntityManager manager{};

    EXPECT_DEATH({ manager.destroyEntity(1); }, "Entity out of range");
}

} // namespace sfa

