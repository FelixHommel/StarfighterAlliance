#include "ecs/systems/UITransformSystem.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

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

TEST_CASE("UITransformSystemTest", "[.][integration][ecs]")
{
    using Catch::Matchers::WithinRel;

    ComponentRegistry registry;

    registry.addComponent<UIHierarchyComponent>(::ROOT, { .parent = NULL_ENTITY, .children = { ::CHILD } });
    registry.addComponent<UITransformComponent>(
        ::ROOT, { .localPosition = ::ROOT_POSITION, .worldPosition = glm::vec2(0.f), .size = ::ROOT_SIZE }
    );
    registry.addComponent<UITransformComponent>(
        ::CHILD, { .localPosition = ::CHILD_POSITION, .worldPosition = glm::vec2(0.f), .size = ::CHILD_SIZE }
    );

    UITransformSystem::update(registry);

    const auto& rootTransform{ registry.getComponent<UITransformComponent>(::ROOT) };
    const auto& childTransform{ registry.getComponent<UITransformComponent>(::CHILD) };

    // NOLINTBEGIN(readability-magic-numbers): to be removed
    CHECK_THAT(rootTransform.worldPosition.x, WithinRel(10.f));
    CHECK_THAT(rootTransform.worldPosition.y, WithinRel(20.f));
    CHECK_THAT(childTransform.worldPosition.x, WithinRel(15.f));
    CHECK_THAT(childTransform.worldPosition.y, WithinRel(27.f));
    // NOLINTEND(readability-magic-numbers)
}

} // namespace sfa::testing

