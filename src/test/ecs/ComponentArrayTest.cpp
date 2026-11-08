#include "ecs/ComponentArray.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/IComponent.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

// NOLINTNEXTLINE(misc-include-cleaner): For some reason clang-tidy sees <algorithm> as unused but it provides std::ranges::fold_left
#include <algorithm>
#include <stdexcept>

namespace
{

constexpr sfa::EntityID TEST_ENTITY_1{ 1 };
constexpr sfa::EntityID TEST_ENTITY_2{ 2 };

constexpr auto TEST_DATA{ 1 };
constexpr auto TEST_DATA_ENTITY_2{ 4 };


/// \brief Component purely used to test the ComponentArray
///
/// \author Felix Hommel
/// \date 1/26/2026
struct TestComponent : public sfa::IComponent
{
    int data{ ::TEST_DATA };
};

} // namespace

namespace sfa::testing
{

/// \brief Test the features of \ref ComponentArray.
///
/// \author Felix Hommel
/// \date 1/26/2026
TEST_CASE("ComponentArrayTest", "[unit][ecs]")
{
    using Catch::Matchers::ContainsSubstring;
    using Catch::Matchers::MessageMatches;

    ComponentArray<TestComponent> array;

    /// \brief Test the Construction of a new \ref ComponentArray.
    ///
    /// After construction a new \ref ComponentArray should have zero elements.
    SECTION("Construction")
    {
        CHECK(array.size() == 0);
    }

    /// \brief Test valid insertions of new Components into the \ref ComponentArray.
    ///
    /// When an entity does not already have the component, insertion should work without problems and increase the size of
    /// the \ref ComponentArray.
    SECTION("Insert into array no duplicate")
    {
        array.insert(TEST_ENTITY_1, {});
        CHECK(array.size() == 1);
        CHECK(array.get(TEST_ENTITY_1).data == TEST_DATA);

        array.insert(TEST_ENTITY_2, { .data = TEST_DATA_ENTITY_2 });
        CHECK(array.size() == 2);
        CHECK(array.get(TEST_ENTITY_2).data == TEST_DATA_ENTITY_2);
    }

    /// \brief Test the insertion of a duplicate component for an entity.
    ///
    /// When a duplicate component is attempted to be inserted, the insert() method should fail an assertion when build in
    /// debug mode.
    SECTION("Insert duplicate into array")
    {
        array.insert(TEST_ENTITY_1, {});

        CHECK_THROWS_MATCHES(
            array.insert(TEST_ENTITY_1, {}),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component already exists"))
        );
    }

    /// \brief Test the removal of an existing component that is at the end of the array.
    ///
    /// When the component exists, removal should reduce the array size by one.
    SECTION("Removing existing component at back")
    {
        array.insert(TEST_ENTITY_1, {});

        array.remove(TEST_ENTITY_1);

        CHECK(array.size() == 0);
    }

    /// \brief Test the removal of an existing component that is not at the end of the array.
    ///
    /// When the removed component is not at the end of the array, then the \ref ComponentArray should preserve its density.
    /// That means, it has to shift all the elements after the removed element one to the left.
    SECTION("Removing existing component not at back")
    {
        array.insert(TEST_ENTITY_1, {});
        array.insert(TEST_ENTITY_2, { .data = TEST_DATA_ENTITY_2 });

        array.remove(TEST_ENTITY_1);

        CHECK(array.size() == 1);
        CHECK(array.get(TEST_ENTITY_2).data == TEST_DATA_ENTITY_2);
        CHECK(array.entityAtIndex(0) == TEST_ENTITY_2);
    }

    /// \brief Test the removal of a non existing component from the \ref ComponentArray.
    ///
    /// If a non existing component is removed the remove() method should fail an assertion when build in debug mode.
    SECTION("Remove non existing component")
    {
        CHECK_THROWS_MATCHES(
            array.remove(TEST_ENTITY_1),
            std::runtime_error,
            MessageMatches(ContainsSubstring("Component doesn't exist"))
        );
    }

    /// \brief Test the read-write access to the \ref ComponentArray.
    ///
    /// The non-const get() method should provide read-write access to the components in the array.
    SECTION("Modify values with get")
    {
        array.insert(TEST_ENTITY_1, {});

        array.get(TEST_ENTITY_1).data = TEST_DATA_ENTITY_2;

        CHECK(array.get(TEST_ENTITY_1).data == TEST_DATA_ENTITY_2);
    }

    /// \brief Test the read-only access with the const get() method.
    ///
    /// The const version of the get() method should provide read-only access to the components.
    SECTION("Read access with const get")
    {
        array.insert(TEST_ENTITY_1, { .data = TEST_DATA_ENTITY_2 });

        const auto& arrayRef{ array };

        CHECK(arrayRef.get(TEST_ENTITY_1).data == TEST_DATA_ENTITY_2);
    }

    /// \brief Test the contains check on a component that is in the \ref ComponentArray.
    ///
    /// When the component has been inserted into the \ref ComponentArray, the contains() method should return true.
    SECTION("Contains on existing component")
    {
        array.insert(TEST_ENTITY_1, {});

        CHECK(array.contains(TEST_ENTITY_1));
    }

    /// \brief Test the contains check on a component that is not in the \ref ComponentArray.
    ///
    /// When the component has not been inserted into the \ref ComponentArray, the contains() method should return false.
    SECTION("Contains on non existing component")
    {
        CHECK(!array.contains(TEST_ENTITY_1));
    }

    /// \brief Test the method to get the entity ID at a certain valid index.
    ///
    /// When a component is inserted into the \ref ComponentArray the entityAtIndex() method should return the EntityID of
    /// the entity a component at a certain index in the \ref ComponentArray belongs to.
    SECTION("Get entity at index")
    {
        array.insert(TEST_ENTITY_1, {});
        array.insert(TEST_ENTITY_2, {});

        CHECK(array.entityAtIndex(0) == TEST_ENTITY_1);
        CHECK(array.entityAtIndex(1) == TEST_ENTITY_2);
    }

    /// \brief Test the method to get the entity ID at a certain invalid index.
    ///
    /// When a component wasn't inserted into the \ref ComponentArray the entityAtIndex() method should fail an assertion,
    /// when build in debug mode
    SECTION("Get entity at invalid index")
    {
        CHECK_THROWS_MATCHES(
            array.entityAtIndex(1), std::runtime_error, MessageMatches(ContainsSubstring("Index is out of range"))
        );
    }

    /// \brief Test the entityDestroyed() method on an existing component
    ///
    /// When the component exists in the \ref ComponentArray, then the entityDestroyed() method should remove the component.
    SECTION("Destroy component if present")
    {
        array.insert(TEST_ENTITY_1, {});

        array.entityDestroyed(TEST_ENTITY_1);

        CHECK(array.size() == 0);
    }

    /// \brief Test the entityDestroyed() method on a non-existing component
    ///
    /// When the component exists in the \ref ComponentArray, then the entityDestroyed() method should remove the component.
    SECTION("Destroy component if not present")
    {
        array.entityDestroyed(TEST_ENTITY_1);

        CHECK(array.size() == 0);
    }

    /// \brief Test the iteration support of \ref ComponentArray.
    ///
    /// When there are no elements inserted in the \ref ComponentArray the begin should equal the end of the \ref ComponentArray.
    SECTION("Empty iteration range")
    {
        CHECK(array.begin() == array.end());
    }

    /// \brief Test the const iteration support of \ref ComponentArray.
    ///
    /// When the \ref ComponentArray is empty, the constant iterators should be the same at the begin and the end.
    SECTION("Empty const Iteration range")
    {
        CHECK(array.cbegin() == array.cend());
    }

    /// \brief Test the iteration support of \ref ComponentArray.
    ///
    /// The \ref ComponentArray should support iteration over it for convenience. When iterated over the \ref ComponentArray
    /// the iteration range should not exceed the amount of actually inserted elements.
    SECTION("Iteration support")
    {
        array.insert(TEST_ENTITY_1, {});
        array.insert(TEST_ENTITY_2, { .data = TEST_DATA_ENTITY_2 });

        // NOLINTNEXTLINE(misc-include-cleaner): clang-tidy says that no header providing std::ranges::fold_left is included, but <alogirthm> is included
        const auto sum{ std::ranges::fold_left(array, 0, [](int accumulator, const TestComponent& comp) {
            return accumulator + comp.data;
        }) };

        CHECK(sum == TEST_DATA + TEST_DATA_ENTITY_2);
    }

    /// \brief Test the range-based for-loop support of \ref ComponentArray.
    ///
    /// The \ref ComponentArray should be able to be iterated over via range-based for-loops for convenience and ease of use.
    SECTION("Range based for loop support")
    {
        array.insert(TEST_ENTITY_1, {});
        array.insert(TEST_ENTITY_2, { .data = TEST_DATA_ENTITY_2 });

        int sum{ 0 };
        for(const auto& c : array)
            sum += c.data;

        CHECK(sum == TEST_DATA + TEST_DATA_ENTITY_2);
    }

    /// \brief Test the span support of \ref ComponentArray
    ///
    /// The \ref ComponentArray should be able to provide a span interface to use as iteration interface. The span should not
    /// exceed the size of the actually inserted elements.
    SECTION("Span support")
    {
        array.insert(TEST_ENTITY_1, {});
        array.insert(TEST_ENTITY_2, { .data = TEST_DATA_ENTITY_2 });

        // NOLINTNEXTLINE(misc-include-cleaner): clang-tidy says that no header providing std::ranges::fold_left is included, but <alogirthm> is included
        const auto sum{ std::ranges::fold_left(array.span(), 0, [](int accumulator, const TestComponent& comp) {
            return accumulator + comp.data;
        }) };

        CHECK(sum == TEST_DATA + TEST_DATA_ENTITY_2);
    }

    /// \brief Test the const span support of \ref ComponentArray
    ///
    /// The \ref ComponentArray should be able to provide a const span interface to use as iteration interface. The span
    /// should not exceed the size of the actually inserted elements.
    SECTION("Const span support")
    {
        ComponentArray<TestComponent> array;
        array.insert(TEST_ENTITY_1, {});
        array.insert(TEST_ENTITY_2, { .data = TEST_DATA_ENTITY_2 });

        const auto& arrayRef{ array };

        const auto spanSize{ arrayRef.span().size() };
        CHECK(spanSize == 2);
    }
}

} // namespace sfa::testing

