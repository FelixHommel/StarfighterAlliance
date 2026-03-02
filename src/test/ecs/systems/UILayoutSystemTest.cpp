#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UILayoutComponent.hpp"
#include "ecs/components/UILayoutElementComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"
#include "ecs/systems/LayoutSystem.hpp"

#include <glm/glm.hpp>

#include <gtest/gtest.h>

namespace
{

constexpr sfa::EntityID PARENT{ 1 };
constexpr sfa::EntityID CHILD_A{ 2 };
constexpr sfa::EntityID CHILD_B{ 3 };

constexpr glm::vec2 PARENT_SIZE{ 100.f };
constexpr glm::vec2 CHILDREN_SIZE{ 1.f, 20.f };

constexpr auto LAYOUT_SPACING{ 10.f };
constexpr glm::vec2 LAYOUT_PADDING{ 10.f };

constexpr auto FLEX_GROW_CHILD_A{ 1.f };
constexpr auto FLEX_GROW_CHILD_B{ 3.f };

} // namespace

namespace sfa::testing
{

TEST(UILayoutSystemTest, VerticalLayoutDistributesFreeSpaceByFlexGrow)
{
    ComponentRegistry registry;

    // NOTE: Add Transform components
    registry.addComponent<UITransformComponent>(
        ::PARENT, { .localPosition = glm::vec2(0.f), .worldPosition = glm::vec2(0.f), .size = ::PARENT_SIZE }
    );
    registry.addComponent<UITransformComponent>(
        ::CHILD_A, { .localPosition = glm::vec2(0.f), .worldPosition = glm::vec2(0.f), .size = ::CHILDREN_SIZE }
    );
    registry.addComponent<UITransformComponent>(
        ::CHILD_B, { .localPosition = glm::vec2(0.f), .worldPosition = glm::vec2(0.f), .size = ::CHILDREN_SIZE }
    );

    // NOTE: Add Root element properties
    registry.addComponent<UIHierarchyComponent>(
        ::PARENT, { .parent = NULL_ENTITY, .children = { ::CHILD_A, ::CHILD_B } }
    );
    registry.addComponent<UILayoutComponent>(
        ::PARENT, { .type = UILayoutComponent::Type::Vertical, .spacing = ::LAYOUT_SPACING, .padding = ::LAYOUT_PADDING }
    );

    // NOTE: Add Child element properties
    registry.addComponent<UILayoutElementComponent>(
        ::CHILD_A, { .preferredSize = CHILDREN_SIZE, .flexGrow = FLEX_GROW_CHILD_A }
    );
    registry.addComponent<UILayoutElementComponent>(
        ::CHILD_B, { .preferredSize = CHILDREN_SIZE, .flexGrow = FLEX_GROW_CHILD_B }
    );

    LayoutSystem::update(registry);

    const auto& first{ registry.getComponent<UITransformComponent>(CHILD_A) };
    const auto& second{ registry.getComponent<UITransformComponent>(CHILD_B) };

    EXPECT_FLOAT_EQ(10.f, first.localPosition.x);
    EXPECT_FLOAT_EQ(10.f, first.localPosition.y);
    EXPECT_FLOAT_EQ(80.f, first.size.x);
    EXPECT_FLOAT_EQ(27.5f, first.size.y);

    EXPECT_FLOAT_EQ(10.f, second.localPosition.x);
    EXPECT_FLOAT_EQ(47.5f, second.localPosition.y);
    EXPECT_FLOAT_EQ(80.f, second.size.x);
    EXPECT_FLOAT_EQ(42.5f, second.size.y);
}

} // namespace sfa::testing

