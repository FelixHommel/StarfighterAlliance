#include "ecs/EntityManager.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <cstddef>
#include <stdexcept>

namespace sfa
{

/// \brief Test the \ref EntityManager class.
///
/// \author Felix Hommel
/// \date 1/27/2026
TEST_CASE("EntityManagerTest", "[unit][ecs]")
{
    using Catch::Matchers::ContainsSubstring;
    using Catch::Matchers::MessageMatches;

    EntityManager manager{};

    /// \brief Test the Construction of a \ref EntityManager.
    ///
    /// After the \re EntityManager is constructed, it should not have allocated any entities yet.
    SECTION("Construction")
    {
        CHECK(manager.livingEntityCount() == 0);
    }

    /// \brief Test the creation of new entities in the \ref EntityManager.
    ///
    /// When an entity is created, the living entity count is increased. Entity IDs are ascending.
    SECTION("Create entity")
    {
        const auto id{ manager.createEntity() };

        CHECK(id == 1);
        CHECK(manager.livingEntityCount() == 1);
    }

    /// \brief Test the behavior when the \ref EntityManager has already created its maximum of entities.
    ///
    /// When the maximum of entities already is created, the createEntity() method should fail an assertion when build in
    /// debug mode.
    SECTION("Create entity exceeds limit")
    {
        for(std::size_t i{ 0 }; i < EntityManager::maxEntities(); ++i)
            auto x{ manager.createEntity() };

        CHECK_THROWS_MATCHES(
            manager.createEntity(), std::runtime_error, MessageMatches(ContainsSubstring("Too many entities"))
        );
    }

    /// \brief Test the behavior of the destroyEntity() method on a before created entity.
    ///
    /// When an entity that was previously created is destroyed, the living entity count should be decreased by one.
    SECTION("Destroy entities")
    {
        const auto entity{ manager.createEntity() };

        manager.destroyEntity(entity);

        CHECK(manager.livingEntityCount() == 0);
    }

    /// \brief Test the behavior of the destroyEntity() method when attempting to destroy an entity that never was created.
    ///
    /// When an entity that never was created is attempted to be deleted, the destroyEntity() method should fail an
    /// assertion when build in debug mode.
    SECTION("Destroy Non existing entitie")
    {
        CHECK_THROWS_MATCHES(
            manager.destroyEntity(1), std::runtime_error, MessageMatches(ContainsSubstring("Entity out of range"))
        );
    }
}

} // namespace sfa

