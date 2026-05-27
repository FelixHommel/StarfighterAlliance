#include "concurrency/AsyncValue.hpp"

#include <gtest/gtest.h>

#include <cstdint>

namespace
{

enum class TestEnum : std::uint8_t
{
    Value1,
    Value2
};

constexpr int DEFAULT_INT_VALUE{ 124 };
constexpr int SET_INT_VALUE{ 971 };

constexpr auto DEFAULT_ENUM_VALUE{ TestEnum::Value1 };
constexpr auto SET_ENUM_VALUE{ TestEnum::Value2 };

} // namespace

namespace sfa::testing
{

/// \brief Test \ref AsyncValue with a default constructed initial value
///
/// In this case AsyncValue::ready() should be false since it never was set and the value stored is a default constructed
/// integer.
TEST(AsyncValueTest, ConstructWithoutDefaultIntValue)
{
    AsyncValue<int> value{};
    int referenceValue{};

    EXPECT_FALSE(value.ready());
    EXPECT_EQ(value.get(), referenceValue);
}

/// \brief Test \ref AsyncValue with an explicitly specified initial value
///
/// In this case AsyncValue::ready() should be false, since AsyncValue::set() was never called.The value from
/// AsyncValue::get() should be the same as the specified initial value.
TEST(AsyncValueTest, ConstructWithDefaultIntValue)
{
    AsyncValue<int> value{ ::DEFAULT_INT_VALUE };

    EXPECT_FALSE(value.ready());
    EXPECT_EQ(value.get(), ::DEFAULT_INT_VALUE);
}

/// \brief Test \ref AsyncValue by setting a value
///
/// In this case AsyncValue::ready() should be true and AsyncValue::get() should return the value set with AsyncValue::set()
TEST(AsyncValueTest, SetAsyncIntValue)
{
    AsyncValue<int> value{ ::DEFAULT_INT_VALUE };
    value.set(::SET_INT_VALUE);

    EXPECT_TRUE(value.ready());
    EXPECT_EQ(value.get(), ::SET_INT_VALUE);
}

/// \brief Test \ref AsyncValue with a default constructed initial value
///
/// In this case AsyncValue::ready() should be false since it never was set and the value stored is a default constructed
/// enum value.
TEST(AsyncValueTest, ConstructWithoutDefaultEnumValue)
{
    AsyncValue<::TestEnum> value{};
    ::TestEnum referenceValue{};

    EXPECT_FALSE(value.ready());
    EXPECT_EQ(value.get(), referenceValue);
}

/// \brief Test \ref AsyncValue with an explicitly specified initial value
///
/// In this case AsyncValue::ready() should be false, since AsyncValue::set() was never called.The value from
/// AsyncValue::get() should be the same as the specified initial value.
TEST(AsyncValueTest, ConstructWithDefaultEnumValue)
{
    AsyncValue<::TestEnum> value{ ::DEFAULT_ENUM_VALUE };

    EXPECT_FALSE(value.ready());
    EXPECT_EQ(value.get(), ::DEFAULT_ENUM_VALUE);
}

/// \brief Test \ref AsyncValue by setting a value
///
/// In this case AsyncValue::ready() should be true and AsyncValue::get() should return the value set with AsyncValue::set()
TEST(AsyncValueTest, SetAsyncEnumValue)
{
    AsyncValue<::TestEnum> value{ ::DEFAULT_ENUM_VALUE };
    value.set(::SET_ENUM_VALUE);

    EXPECT_TRUE(value.ready());
    EXPECT_EQ(value.get(), ::SET_ENUM_VALUE);
}

} // namespace sfa::testing
