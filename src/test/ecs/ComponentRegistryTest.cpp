#include "ecs/ComponentRegistry.hpp"

#include "ecs/ECSUtility.hpp"
#include "ecs/components/IComponent.hpp"

#include <gtest/gtest.h>

namespace sfa
{

/// \brief Test the features of the \ref ComponentRegistry class
///
/// \author Felix Hommel
/// \date 1/28/2026
class ComponentRegistryTest : public ::testing::Test
{
public:
    static constexpr auto DEFAULT_DATA{ 1 };

    ComponentRegistryTest() = default;
    ~ComponentRegistryTest() override = default;

    ComponentRegistryTest(const ComponentRegistryTest&) = delete;
    ComponentRegistryTest& operator=(const ComponentRegistryTest&) = delete;
    ComponentRegistryTest(ComponentRegistryTest&&) = delete;
    ComponentRegistryTest& operator=(ComponentRegistryTest&&) = delete;

protected:
    static constexpr EntityID ENTITY_1{ 1 };
};

/// \brief Simple test component to conduct unit tests with
///
/// \author Felix Hommel
/// \date 1/28/2026
struct TestComponent : public IComponent
{
    int data{ ComponentRegistryTest::DEFAULT_DATA };
};

/// \brief Test the construction of \ref ComponentRegistry.
///
/// After a new \ref ComponentRegistry is constructed, it should not have any registered components.
TEST_F(ComponentRegistryTest, ComponentRegistryConstruction)
{
    ComponentRegistry manager;

    EXPECT_EQ(manager.registeredComponents(), 0);
}

/// \brief Test the registration of new components to the \ref ComponentRegistry.
///
/// When the component has not been registered before, the \ref ComponentRegistry should register the new component which
/// increases the registered components by one.
TEST_F(ComponentRegistryTest, RegisteringNewComponent)
{
    ComponentRegistry manager;

    manager.registerComponent<TestComponent>();

    EXPECT_EQ(manager.registeredComponents(), 1);
}

/// \brief Test the behavior when trying to register a component that already was registered.
///
/// When trying to register a component that already is registered, the \ref ComponentRegistry should fail an assertion
/// when build in debug mode.
TEST_F(ComponentRegistryTest, AttemptRegisteringDuplicateComponent)
{
    ComponentRegistry manager;
    manager.registerComponent<TestComponent>();

    EXPECT_DEATH({ manager.registerComponent<TestComponent>(); }, "Component already registered");
}

/// \brief Test adding an already registered component to an entity.
///
/// When the component already has been registered the component can be added to the entity without registering it before.
TEST_F(ComponentRegistryTest, AddRegisteredComponentToEntity)
{
    ComponentRegistry manager;
    manager.registerComponent<TestComponent>();

    manager.addComponent<TestComponent>(ENTITY_1, {});

    EXPECT_EQ(manager.registeredComponents(), 1);
    EXPECT_EQ(manager.getComponent<TestComponent>(ENTITY_1).data, DEFAULT_DATA);
}

/// \brief Test adding of a not-registered component to an entity.
///
/// If the component wasn't registered previously, the addComponent() method should register the component before adding
/// it to the entity.
TEST_F(ComponentRegistryTest, AddNotRegisteredComponentToEntity)
{
    ComponentRegistry manager;

    manager.addComponent<TestComponent>(ENTITY_1, {});

    EXPECT_EQ(manager.registeredComponents(), 1);
    EXPECT_TRUE(manager.contains<TestComponent>(ENTITY_1));
    EXPECT_EQ(manager.getComponent<TestComponent>(ENTITY_1).data, DEFAULT_DATA);
}

/// \brief Test the removal of components from entities.
///
/// When a component is removed from an entity, the entity should no longer have the component but the component should
/// still be registered.
TEST_F(ComponentRegistryTest, RemoveRegisteredComponentFromEntity)
{
    ComponentRegistry manager;
    manager.addComponent<TestComponent>(ENTITY_1, {});

    manager.removeComponent<TestComponent>(ENTITY_1);

    EXPECT_EQ(manager.registeredComponents(), 1);
    EXPECT_FALSE(manager.contains<TestComponent>(ENTITY_1));
}

/// \brief Test the removal of a component that has not been registered.
///
/// When a component wasn't registered and is attempted to be removed the removeComponent() method should fail an
/// assertion when build in debug mode.
TEST_F(ComponentRegistryTest, RemoveNotRegisteredComponent)
{
    ComponentRegistry manager;

    EXPECT_DEATH({ manager.removeComponent<TestComponent>(ENTITY_1); }, "Component is not registered");
}

/// \brief Test the removal of a component that was registered but the entity does not have the component.
///
/// When the component was registered prior to the removal attempt but the entity it is removed from never added it,
/// the removeComponent() method should fail an assertion when build in debug mode.
TEST_F(ComponentRegistryTest, RemoveRegisteredComponentFromEntityWithoutComponent)
{
    ComponentRegistry manager;
    manager.registerComponent<TestComponent>();

    EXPECT_DEATH({ manager.removeComponent<TestComponent>(ENTITY_1); }, "The entity does not have the component");
}

/// \brief Test getting not registered component.
///
/// When trying to get a not registered component, the getComponent() method should fail an assertion when build in
/// debug mode.
TEST_F(ComponentRegistryTest, GetNotRegisteredComponent)
{
    ComponentRegistry manager;

    EXPECT_DEATH({ manager.getComponent<TestComponent>(ENTITY_1); }, "Component is not registered");
}

/// \brief Test getting component from an entity that does not have the component.
///
/// When trying to get the component from an entity that does not have the component, the getComponent() method should
/// fail an assertion when build in debug mode.
TEST_F(ComponentRegistryTest, GetComponentFromEntityWithoutComponent)
{
    ComponentRegistry manager;
    manager.registerComponent<TestComponent>();

    EXPECT_DEATH({ manager.getComponent<TestComponent>(ENTITY_1); }, "The entity does not have the component");
}

/// \brief Test getting const-ref component.
///
/// The \ref ComponentRegistry should provide const-ref access to components. When trying to get a registered component
/// from an entity that has the component, it should return a const-ref to the component.
TEST_F(ComponentRegistryTest, ConstGetComponent)
{
    ComponentRegistry manager;
    manager.addComponent<TestComponent>(ENTITY_1, {});
    const auto& refManager{ manager };

    EXPECT_EQ(refManager.getComponent<TestComponent>(ENTITY_1).data, DEFAULT_DATA);
}

/// \brief Test getting cont-ref component for not registered component.
///
/// When trying to get a component that was not registered before, getComponent() should fail an assertion when build
/// in debug mode.
TEST_F(ComponentRegistryTest, ConstGetNotRegisteredComponent)
{
    ComponentRegistry manager;
    const auto& refManager{ manager };

    EXPECT_DEATH({ refManager.getComponent<TestComponent>(ENTITY_1); }, "Component is not registered");
}

/// \brief Test getting const-ref component from entity that does not have the component.
///
/// When trying to get the component from an entity that does not have the component, the getComponent() method should
/// fail an assertion when build in debug mode.
TEST_F(ComponentRegistryTest, ConstGetComponentFromEntityWithoutComponent)
{
    ComponentRegistry manager;
    manager.registerComponent<TestComponent>();
    const auto& refManager{ manager };

    EXPECT_DEATH({ refManager.getComponent<TestComponent>(ENTITY_1); }, "The entity does not have the component");
}

/// \brief Test checking if an entity has a component.
///
/// When checking if an entity has a component and the entity has added the component prior to the check, the contains()
/// method should return true.
TEST_F(ComponentRegistryTest, ContainsComponent)
{
    ComponentRegistry manager;
    manager.addComponent<TestComponent>(ENTITY_1, {});

    EXPECT_TRUE(manager.contains<TestComponent>(ENTITY_1));
}

/// \brief Test checking if an entity has a component.
///
/// When checking if an entity has a component and the entity has not added the component prior to the check, the
/// contains() method should return false.
TEST_F(ComponentRegistryTest, DoesNotContainComponent)
{
    ComponentRegistry manager;
    manager.registerComponent<TestComponent>();

    EXPECT_FALSE(manager.contains<TestComponent>(ENTITY_1));
}

// \brief Test checking if an entity has a component that was never registered.
//
// When checking if an entity has a component but the component was never registered, the contains() method should fail
// an assertion when build in debug mode.
TEST_F(ComponentRegistryTest, ContainsNotRegisteredComponent)
{
    ComponentRegistry manager;

    EXPECT_DEATH({ manager.contains<TestComponent>(ENTITY_1); }, "Component is not registered");
}

/// \brief The \ref ComponentRegistry should provide the ability to retrieve an entire \ref ComponentArray for one component.
///
/// The getComponentArray() method should return all components of a certain type.
TEST_F(ComponentRegistryTest, GetComponentArray)
{
    ComponentRegistry manager;
    manager.addComponent<TestComponent>(ENTITY_1, {});

    EXPECT_EQ(manager.getComponentArray<TestComponent>().size(), 1);
}

/// \brief Test trying to get the \ref ComponentArray of a component that was not registered.
///
/// When trying to get the \ref ComponentArray of a component that was not registered previously, the getComponentArray
/// should fail an assertion when build in debug mode.
TEST_F(ComponentRegistryTest, GetComponentArrayForNotRegisteredComponent)
{
    ComponentRegistry manager;

    EXPECT_DEATH({ manager.getComponentArray<TestComponent>(); }, "Component is not registered");
}

/// \brief The \ref ComponentRegistry should provide an interface to access the \ref ComponentArray of a certain component.
///
/// The getComponentArray() method should return a const ref to the \ref ComponentArray when called from a const context.
TEST_F(ComponentRegistryTest, ConstGetComponentArray)
{
    ComponentRegistry manager;
    manager.addComponent<TestComponent>(ENTITY_1, {});
    const auto& refManager{ manager };

    EXPECT_EQ(refManager.getComponentArray<TestComponent>().size(), 1);
}

/// \brief Test trying to get const ref access to a \ref ComponentArray of a component that was not registered previosuly.
///
/// When trying to get the \ref ComponentArray of a component that was not registered previously, the getComponentArray()
/// method should fail an assertion when build in debug mode.
TEST_F(ComponentRegistryTest, ConstGetComponentArrayForNotRegisteredComponent)
{
    ComponentRegistry manager;
    manager.addComponent<TestComponent>(ENTITY_1, {});
    const auto& refManager{ manager };

    EXPECT_DEATH({ refManager.getComponentArray<TestComponent>(); }, "Component is not registered");
}

/// \brief Test the removal of all components from an entity.
///
/// When removing all components from an entity, the entity should not have any components attached to it at the end
/// and all affected \ref ComponentArray should be decreased in size by one.
TEST_F(ComponentRegistryTest, RemoveAllComponentsOfEntity)
{
    ComponentRegistry manager;
    manager.addComponent<TestComponent>(ENTITY_1, {});

    manager.entityDestroyed(ENTITY_1);

    EXPECT_FALSE(manager.contains<TestComponent>(ENTITY_1));
}

/// \brief Test the removal of all components from an entity that has no components assigned to it.
///
/// When the targeted component has no components assigned to it in the first place, nothing should change.
TEST_F(ComponentRegistryTest, RemoveAllComponentsOfEntityThatHasNoComponents)
{
    ComponentRegistry manager;

    manager.entityDestroyed(ENTITY_1);

    EXPECT_FALSE(manager.contains<TestComponent>(ENTITY_1));
}

} // namespace sfa

