#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/IComponent.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <stdexcept>

namespace
{

constexpr sfa::EntityID TEST_ENTITY{ 1 };
constexpr auto TEST_DATA{ 1 };

/// \brief Simple test component to conduct unit tests with
///
/// \author Felix Hommel
/// \date 1/28/2026
struct TestComponent : public sfa::IComponent
{
    int data{ ::TEST_DATA };
};

} // namespace

namespace sfa
{

/// \brief Test the features of the \ref ComponentRegistry class
///
/// \author Felix Hommel
/// \date 1/28/2026
TEST_CASE("ComponentRegistryTest", "[unit][ecs]")
{
    ComponentRegistry manager;

    /// \brief Test the construction of \ref ComponentRegistry.
    ///
    /// After a new \ref ComponentRegistry is constructed, it should not have any registered components.
    SECTION("Component registry construction")
    {
        CHECK(manager.registeredComponents() == 0);
    }

    /// \brief Test the registration of new components to the \ref ComponentRegistry.
    ///
    /// When the component has not been registered before, the \ref ComponentRegistry should register the new component which
    /// increases the registered components by one.
    SECTION("Registering new component")
    {
        manager.registerComponent<TestComponent>();

        CHECK(manager.registeredComponents() == 1);
    }

    /// \brief Test the behavior when trying to register a component that already was registered.
    ///
    /// When trying to register a component that already is registered, the \ref ComponentRegistry should fail an assertion
    /// when build in debug mode.
    SECTION("Attempt registering duplicate component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        manager.registerComponent<TestComponent>();

        CHECK_THROWS_MATCHES(
            manager.registerComponent<TestComponent>(),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component already registered"))
        );
    }

    /// \brief Test adding an already registered component to an entity.
    ///
    /// When the component already has been registered the component can be added to the entity without registering it before.
    SECTION("Add registered component to entity")
    {
        manager.registerComponent<TestComponent>();

        manager.addComponent<TestComponent>(::TEST_ENTITY, {});

        CHECK(manager.registeredComponents() == 1);
        CHECK(manager.getComponent<TestComponent>(::TEST_ENTITY).data == ::TEST_DATA);
    }

    /// \brief Test adding of a not-registered component to an entity.
    ///
    /// If the component wasn't registered previously, the addComponent() method should register the component before adding
    /// it to the entity.
    SECTION("Add not registered component to entity")
    {
        manager.addComponent<TestComponent>(::TEST_ENTITY, {});

        CHECK(manager.registeredComponents() == 1);
        CHECK(manager.contains<TestComponent>(::TEST_ENTITY));
        CHECK(manager.getComponent<TestComponent>(::TEST_ENTITY).data == ::TEST_DATA);
    }

    /// \brief Test the removal of components from entities.
    ///
    /// When a component is removed from an entity, the entity should no longer have the component but the component should
    /// still be registered.
    SECTION("Remove registered component from entity")
    {
        manager.addComponent<TestComponent>(::TEST_ENTITY, {});

        manager.removeComponent<TestComponent>(::TEST_ENTITY);

        CHECK(manager.registeredComponents() == 1);
        CHECK(!manager.contains<TestComponent>(::TEST_ENTITY));
    }

    /// \brief Test the removal of a component that has not been registered.
    ///
    /// When a component wasn't registered and is attempted to be removed the removeComponent() method should fail an
    /// assertion when build in debug mode.
    SECTION("Remove not registered component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        CHECK_THROWS_MATCHES(
            manager.removeComponent<TestComponent>(::TEST_ENTITY),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component is not registered"))
        );
    }

    /// \brief Test the removal of a component that was registered but the entity does not have the component.
    ///
    /// When the component was registered prior to the removal attempt but the entity it is removed from never added it,
    /// the removeComponent() method should fail an assertion when build in debug mode.
    SECTION("Remove registered component from entity without component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        manager.registerComponent<TestComponent>();

        CHECK_THROWS_MATCHES(
            manager.removeComponent<TestComponent>(::TEST_ENTITY),
            std::runtime_error,
            MessageMatches(ContainsSubstring("The entity does not have the component"))
        );
    }

    /// \brief Test getting not registered component.
    ///
    /// When trying to get a not registered component, the getComponent() method should fail an assertion when build in
    /// debug mode.
    SECTION("Get not registered component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        CHECK_THROWS_MATCHES(
            manager.getComponent<TestComponent>(::TEST_ENTITY),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component is not registered"))
        );
    }

    /// \brief Test getting component from an entity that does not have the component.
    ///
    /// When trying to get the component from an entity that does not have the component, the getComponent() method should
    /// fail an assertion when build in debug mode.
    SECTION("Get component from entity without component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        manager.registerComponent<TestComponent>();

        CHECK_THROWS_MATCHES(
            manager.getComponent<TestComponent>(::TEST_ENTITY),
            std::runtime_error,
            MessageMatches(ContainsSubstring("The entity does not have the component"))
        );
    }

    /// \brief Test getting const-ref component.
    ///
    /// The \ref ComponentRegistry should provide const-ref access to components. When trying to get a registered component
    /// from an entity that has the component, it should return a const-ref to the component.
    SECTION("Const get component")
    {
        manager.addComponent<TestComponent>(::TEST_ENTITY, {});
        const auto& refManager{ manager };

        CHECK(refManager.getComponent<TestComponent>(::TEST_ENTITY).data == ::TEST_DATA);
    }

    /// \brief Test getting cont-ref component for not registered component.
    ///
    /// When trying to get a component that was not registered before, getComponent() should fail an assertion when build
    /// in debug mode.
    SECTION("Const get not registered component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        const auto& refManager{ manager };

        CHECK_THROWS_MATCHES(
            refManager.getComponent<TestComponent>(::TEST_ENTITY),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component is not registered"))
        );
    }

    /// \brief Test getting const-ref component from entity that does not have the component.
    ///
    /// When trying to get the component from an entity that does not have the component, the getComponent() method should
    /// fail an assertion when build in debug mode.
    SECTION("Const get component from entity without component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        manager.registerComponent<TestComponent>();
        const auto& refManager{ manager };

        CHECK_THROWS_MATCHES(
            refManager.getComponent<TestComponent>(::TEST_ENTITY),
            std::runtime_error,
            MessageMatches(ContainsSubstring("The entity does not have the component"))
        );
    }

    /// \brief Test checking if an entity has a component.
    ///
    /// When checking if an entity has a component and the entity has added the component prior to the check, the contains()
    /// method should return true.
    SECTION("Contains component")
    {
        manager.addComponent<TestComponent>(::TEST_ENTITY, {});

        CHECK(manager.contains<TestComponent>(::TEST_ENTITY));
    }

    /// \brief Test checking if an entity has a component.
    ///
    /// When checking if an entity has a component and the entity has not added the component prior to the check, the
    /// contains() method should return false.
    SECTION("Does not contain component")
    {
        manager.registerComponent<TestComponent>();

        CHECK(!manager.contains<TestComponent>(::TEST_ENTITY));
    }

    // \brief Test checking if an entity has a component that was never registered.
    //
    // When checking if an entity has a component but the component was never registered, the contains() method should fail
    // an assertion when build in debug mode.
    SECTION("Contains not registered component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        CHECK_THROWS_MATCHES(
            manager.contains<TestComponent>(::TEST_ENTITY),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component is not registered"))
        );
    }

    /// \brief The \ref ComponentRegistry should provide the ability to retrieve an entire \ref ComponentArray for one component.
    ///
    /// The getComponentArray() method should return all components of a certain type.
    SECTION("Get component array")
    {
        manager.addComponent<TestComponent>(::TEST_ENTITY, {});

        CHECK(manager.getComponentArray<TestComponent>().size() == 1);
    }

    /// \brief Test trying to get the \ref ComponentArray of a component that was not registered.
    ///
    /// When trying to get the \ref ComponentArray of a component that was not registered previously, the getComponentArray
    /// should fail an assertion when build in debug mode.
    SECTION("Get component array for not registered component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        CHECK_THROWS_MATCHES(
            manager.getComponentArray<TestComponent>(),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component is not registered"))
        );
    }

    /// \brief The \ref ComponentRegistry should provide an interface to access the \ref ComponentArray of a certain component.
    ///
    /// The getComponentArray() method should return a const ref to the \ref ComponentArray when called from a const context.
    SECTION("Const get component array")
    {
        manager.addComponent<TestComponent>(::TEST_ENTITY, {});
        const auto& refManager{ manager };

        CHECK(refManager.getComponentArray<TestComponent>().size() == 1);
    }

    /// \brief Test trying to get const ref access to a \ref ComponentArray of a component that was not registered previously.
    ///
    /// When trying to get the \ref ComponentArray of a component that was not registered previously, the getComponentArray()
    /// method should fail an assertion when build in debug mode.
    SECTION("Const get component array for not registered component")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        const auto& refManager{ manager };

        CHECK_THROWS_MATCHES(
            refManager.getComponentArray<TestComponent>(),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component is not registered"))
        );
    }

    /// \brief Test the removal of all components from an entity.
    ///
    /// When removing all components from an entity, the entity should not have any components attached to it at the end
    /// and all affected \ref ComponentArray should be decreased in size by one.
    SECTION("Remove all components of entity")
    {
        manager.addComponent<TestComponent>(::TEST_ENTITY, {});

        manager.entityDestroyed(::TEST_ENTITY);

        CHECK(!manager.contains<TestComponent>(::TEST_ENTITY));
    }

    /// \brief Test the removal of all components from an entity that has no components assigned to it.
    ///
    /// When the targeted component has no components assigned to it in the first place, nothing should change.
    SECTION("Remove all components of entity that has no components")
    {
        manager.registerComponent<TestComponent>();

        manager.entityDestroyed(::TEST_ENTITY);

        CHECK(!manager.contains<TestComponent>(::TEST_ENTITY));
    }
}

} // namespace sfa

