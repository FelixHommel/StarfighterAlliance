#include "ecs/ComponentArray.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/IComponent.hpp"

#include <gtest/gtest.h>

// NOLINTNEXTLINE(misc-include-cleaner): For some reason clang-tidy sees <algorithm> as unused but it provides std::ranges::fold_left
#include <algorithm>

namespace sfa::testing
{

/// \brief Test the features of \ref ComponentArray.
///
/// \author Felix Hommel
/// \date 1/26/2026
class ComponentArrayTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_DATA{ 1 };

    ComponentArrayTest() = default;
    ~ComponentArrayTest() override = default;

    ComponentArrayTest(const ComponentArrayTest&) = delete;
    ComponentArrayTest& operator=(const ComponentArrayTest&) = delete;
    ComponentArrayTest(ComponentArrayTest&&) = delete;
    ComponentArrayTest& operator=(ComponentArrayTest&&) = delete;

protected:
    static constexpr EntityID ENTITY_1{ 1 };
    static constexpr EntityID ENTITY_2{ 2 };
    static constexpr int DATA_ENTITY_2{ 4 };
};

/// \brief Component purely used to test the ComponentArray
///
/// \author Felix Hommel
/// \date 1/26/2026
struct TestComponent : public IComponent
{
    int data{ ComponentArrayTest::DEFAULT_DATA };
};

/// \brief Test the Construction of a new \ref ComponentArray.
///
/// After construction a new \ref ComponentArray should have zero elements.
TEST_F(ComponentArrayTest, Construction)
{
    ComponentArray<TestComponent> array;

    EXPECT_EQ(array.size(), 0);
}

/// \brief Test valid insertions of new Components into the \ref ComponentArray.
///
/// When an entity does not already have the component, insertion should work without problems and increase the size of
/// the \ref ComponentArray.
TEST_F(ComponentArrayTest, InsertIntoArrayNoDuplicate)
{
    ComponentArray<TestComponent> array;

    array.insert(ENTITY_1, {});
    EXPECT_EQ(array.size(), 1);
    EXPECT_EQ(array.get(ENTITY_1).data, DEFAULT_DATA);

    array.insert(ENTITY_2, { .data = DATA_ENTITY_2 });
    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array.get(ENTITY_2).data, DATA_ENTITY_2);
}

/// \brief Test the insertion of a duplicate component for an entity.
///
/// When a duplicate component is attempted to be inserted, the insert() method should fail an assertion when build in
/// debug mode.
TEST_F(ComponentArrayTest, InsertDuplicateIntoArray)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});

    EXPECT_DEATH({ array.insert(ENTITY_1, {}); }, "Component already exists");
}

/// \brief Test the removal of an existing component that is at the end of the array.
///
/// When the component exists, removal should reduce the array size by one.
TEST_F(ComponentArrayTest, RemovingExistingComponentAtBack)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});

    array.remove(ENTITY_1);

    EXPECT_EQ(array.size(), 0);
}

/// \brief Test the removal of an existing component that is not at the end of the array.
///
/// When the removed component is not at the end of the array, then the \ref ComponentArray should preserve its density.
/// That means, it has to shift all the elements after the removed element one to the left.
TEST_F(ComponentArrayTest, RemovingExistingComponentNotAtBack)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});
    array.insert(ENTITY_2, { .data = DATA_ENTITY_2 });

    array.remove(ENTITY_1);

    EXPECT_EQ(array.size(), 1);
    EXPECT_EQ(array.get(ENTITY_2).data, DATA_ENTITY_2);
    EXPECT_EQ(array.entityAtIndex(0), ENTITY_2);
}

/// \brief Test the removal of a non existing component from the \ref ComponentArray.
///
/// If a non existing component is removed the remove() method should fail an assertion when build in debug mode.
TEST_F(ComponentArrayTest, RemoveNonExistingComponent)
{
    ComponentArray<TestComponent> array;

    EXPECT_DEATH({ array.remove(ENTITY_1); }, "Component doesn't exist");
}

/// \brief Test the read-write access to the \ref ComponentArray.
///
/// The non-const get() method should provide read-write access to the components in the array.
TEST_F(ComponentArrayTest, ModifyValuesWithGet)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});

    array.get(ENTITY_1).data = DATA_ENTITY_2;

    EXPECT_EQ(array.get(ENTITY_1).data, DATA_ENTITY_2);
}

/// \brief Test the read-only access with the const get() method.
///
/// The const version of the get() method should provide read-only access to the components.
TEST_F(ComponentArrayTest, ReadAccessWithConstGet)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, { .data = DATA_ENTITY_2 });

    const auto& arrayRef{ array };

    EXPECT_EQ(arrayRef.get(ENTITY_1).data, DATA_ENTITY_2);
}

/// \brief Test the contains check on a component that is in the \ref ComponentArray.
///
/// When the component has been inserted into the \ref ComponentArray, the contains() method should return true.
TEST_F(ComponentArrayTest, ContainsOnExistingComponent)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});

    EXPECT_TRUE(array.contains(ENTITY_1));
}

/// \brief Test the contains check on a component that is not in the \ref ComponentArray.
///
/// When the component has not been inserted into the \ref ComponentArray, the contains() method should return false.
TEST_F(ComponentArrayTest, ContainsOnNonExistingComponent)
{
    ComponentArray<TestComponent> array;

    EXPECT_FALSE(array.contains(ENTITY_1));
}

/// \brief Test the method to get the entity ID at a certain valid index.
///
/// When a component is inserted into the \ref ComponentArray the entityAtIndex() method should return the EntityID of
/// the entity a component at a certain index in the \ref ComponentArray belongs to.
TEST_F(ComponentArrayTest, GetEntityAtIndex)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});
    array.insert(ENTITY_2, {});

    EXPECT_EQ(array.entityAtIndex(0), ENTITY_1);
    EXPECT_EQ(array.entityAtIndex(1), ENTITY_2);
}

/// \brief Test the method to get the entity ID at a certain invalid index.
///
/// When a component wasn't inserted into the \ref ComponentArray the entityAtIndex() method should fail an assertion,
/// when build in debug mode
TEST_F(ComponentArrayTest, GetEntityAtInvalidIndex)
{
    ComponentArray<TestComponent> array;

    EXPECT_DEATH({ array.entityAtIndex(1); }, "Index is out of range");
}

/// \brief Test the entityDestroyed() method on an existing component
///
/// When the component exists in the \ref ComponentArray, then the entityDestroyed() method should remove the component.
TEST_F(ComponentArrayTest, DestroyComponentIfPresent)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});

    array.entityDestroyed(ENTITY_1);

    EXPECT_EQ(array.size(), 0);
}

/// \brief Test the entityDestroyed() method on a non-existing component
///
/// When the component exists in the \ref ComponentArray, then the entityDestroyed() method should remove the component.
TEST_F(ComponentArrayTest, DestroyComponentIfNotPresent)
{
    ComponentArray<TestComponent> array;

    array.entityDestroyed(ENTITY_1);

    EXPECT_EQ(array.size(), 0);
}

/// \brief Test the iteration support of \ref ComponentArray.
///
/// When there are no elements inserted in the \ref ComponentArray the begin should equal the end of the \ref ComponentArray.
TEST_F(ComponentArrayTest, EmptyIterationRange)
{
    ComponentArray<TestComponent> array;

    EXPECT_EQ(array.begin(), array.end());
}

/// \brief Test the const iteration support of \ref ComponentArray.
///
/// When the \ref ComponentArray is empty, the constant iterators should be the same at the begin and the end.
TEST_F(ComponentArrayTest, EmptyConstIterationRange)
{
    ComponentArray<TestComponent> array;

    EXPECT_EQ(array.cbegin(), array.cend());
}

/// \brief Test the iteration support of \ref ComponentArray.
///
/// The \ref ComponentArray should support iteration over it for convenience. When iterated over the \ref ComponentArray
/// the iteration range should not exceed the amount of actually inserted elements.
TEST_F(ComponentArrayTest, IterationSupport)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});
    array.insert(ENTITY_2, { .data = DATA_ENTITY_2 });

    // NOLINTNEXTLINE(misc-include-cleaner): clang-tidy says that no header providing std::ranges::fold_left is included, but <alogirthm> is included
    const auto sum{ std::ranges::fold_left(array, 0, [](int accumulator, const TestComponent& comp) {
        return accumulator + comp.data;
    }) };

    EXPECT_EQ(sum, DEFAULT_DATA + DATA_ENTITY_2);
}

/// \brief Test the range-based for-loop support of \ref ComponentArray.
///
/// The \ref ComponentArray should be able to be iterated over via range-based for-loops for convenience and ease of use.
TEST_F(ComponentArrayTest, RangeBasedForLoopSupport)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});
    array.insert(ENTITY_2, { .data = DATA_ENTITY_2 });

    int sum{ 0 };
    for(const auto& c : array)
        sum += c.data;

    EXPECT_EQ(sum, DEFAULT_DATA + DATA_ENTITY_2);
}

/// \brief Test the span support of \ref ComponentArray
///
/// The \ref ComponentArray should be able to provide a span interface to use as iteration interface. The span should not
/// exceed the size of the actually inserted elements.
TEST_F(ComponentArrayTest, SpanSupport)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});
    array.insert(ENTITY_2, { .data = DATA_ENTITY_2 });

    // NOLINTNEXTLINE(misc-include-cleaner): clang-tidy says that no header providing std::ranges::fold_left is included, but <alogirthm> is included
    const auto sum{ std::ranges::fold_left(array.span(), 0, [](int accumulator, const TestComponent& comp) {
        return accumulator + comp.data;
    }) };

    EXPECT_EQ(sum, DEFAULT_DATA + DATA_ENTITY_2);
}

/// \brief Test the const span support of \ref ComponentArray
///
/// The \ref ComponentArray should be able to provide a const span interface to use as iteration interface. The span
/// should not exceed the size of the actually inserted elements.
TEST_F(ComponentArrayTest, ConstSpanSupport)
{
    ComponentArray<TestComponent> array;
    array.insert(ENTITY_1, {});
    array.insert(ENTITY_2, { .data = DATA_ENTITY_2 });

    const auto& arrayRef{ array };

    const auto spanSize{ arrayRef.span().size() };
    EXPECT_EQ(spanSize, 2);
}

} // namespace sfa::testing

