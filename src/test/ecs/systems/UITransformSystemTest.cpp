#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"
#include "ecs/systems/UITransformSystem.hpp"

#include <gtest/gtest.h>

namespace
{

constexpr sfa::EntityID ROOT{ 1 };
constexpr sfa::EntityID CHILD{ 2 };

constexpr glm::vec2 ROOT_POSITION{ 10.f, 20.f };
constexpr glm::vec2 ROOT_SIZE{ 100.f, 50.f };
constexpr glm::vec2 CHILD_POSITION{ 5.f, 7.f };
constexpr glm::vec2 CHILD_SIZE{ 10.f };

} // namespace

namespace sfa::testing
{

TEST(UITransformSystemTest, ChildWithoutHierarchyStillGetsWorldPositionFromParent)
{
    ComponentRegistry registry;

    registry.addComponent<UIHierarchyComponent>(::ROOT, { .parent = NULL_ENTITY, .children = { ::CHILD } });
    registry.addComponent<UITransformComponent>(
        ::ROOT,
        { .localPosition = ::ROOT_POSITION, .worldPosition = glm::vec2(0.f), .size = ::ROOT_SIZE }
    );
    registry.addComponent<UITransformComponent>(
        ::CHILD,
        { .localPosition = ::CHILD_POSITION, .worldPosition = glm::vec2(0.f), .size = ::CHILD_SIZE }
    );

    UITransformSystem::update(registry);

    const auto& rootTransform{ registry.getComponent<UITransformComponent>(::ROOT) };
    const auto& childTransform{ registry.getComponent<UITransformComponent>(::CHILD) };

    EXPECT_FLOAT_EQ(10.f, rootTransform.worldPosition.x);
    EXPECT_FLOAT_EQ(20.f, rootTransform.worldPosition.y);
    EXPECT_FLOAT_EQ(15.f, childTransform.worldPosition.x);
    EXPECT_FLOAT_EQ(27.f, childTransform.worldPosition.y);
}

} // namespace sfa::testing

