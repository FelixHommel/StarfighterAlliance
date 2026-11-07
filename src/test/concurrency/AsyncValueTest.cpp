#include "concurrency/AsyncValue.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>

namespace
{

enum class AsyncValueTestEnum : std::uint8_t
{
    Value1,
    Value2
};

constexpr int DEFAULT_INT_VALUE{ 124 };
constexpr int SET_INT_VALUE{ 971 };

constexpr auto DEFAULT_ENUM_VALUE{ AsyncValueTestEnum::Value1 };
constexpr auto SET_ENUM_VALUE{ AsyncValueTestEnum::Value2 };

} // namespace

namespace sfa::testing
{

TEST_CASE("AsyncValueTest", "[unit][concurrency]")
{
    /// \brief Test \ref AsyncValue with a default constructed initial value
    ///
    /// In this case AsyncValue::ready() should be false since it never was set and the value stored is a default constructed
    /// integer.
    SECTION("Construct without default int value")
    {
        AsyncValue<int> value{};
        int referenceValue{};

        CHECK(value.ready());
        CHECK(value.get() == referenceValue);
    }

    /// \brief Test \ref AsyncValue with an explicitly specified initial value
    ///
    /// In this case AsyncValue::ready() should be false, since AsyncValue::set() was never called.The value from
    /// AsyncValue::get() should be the same as the specified initial value.
    SECTION("Construct with default int value")
    {
        AsyncValue<int> value{ ::DEFAULT_INT_VALUE };

        CHECK(value.ready());
        CHECK(value.get() == ::DEFAULT_INT_VALUE);
    }

    /// \brief Test \ref AsyncValue by setting a value
    ///
    /// In this case AsyncValue::ready() should be true and AsyncValue::get() should return the value set with AsyncValue::set()
    SECTION("Set async int value")
    {
        AsyncValue<int> value{ ::DEFAULT_INT_VALUE };
        value.set(::SET_INT_VALUE);

        CHECK(value.ready());
        CHECK(value.get() == ::SET_INT_VALUE);
    }

    /// \brief Test \ref AsyncValue with a default constructed initial value
    ///
    /// In this case AsyncValue::ready() should be false since it never was set and the value stored is a default constructed
    /// enum value.
    SECTION("Construct without default enum value")
    {
        AsyncValue<::AsyncValueTestEnum> value{};
        ::AsyncValueTestEnum referenceValue{};

        CHECK(value.ready());
        CHECK(value.get() == referenceValue);
    }

    /// \brief Test \ref AsyncValue with an explicitly specified initial value
    ///
    /// In this case AsyncValue::ready() should be false, since AsyncValue::set() was never called.The value from
    /// AsyncValue::get() should be the same as the specified initial value.
    SECTION("Construct with default enum value")
    {
        AsyncValue<::AsyncValueTestEnum> value{ ::DEFAULT_ENUM_VALUE };

        CHECK(value.ready());
        CHECK(value.get() == ::DEFAULT_ENUM_VALUE);
    }

    /// \brief Test \ref AsyncValue by setting a value
    ///
    /// In this case AsyncValue::ready() should be true and AsyncValue::get() should return the value set with AsyncValue::set()
    SECTION("Set async enum value")
    {
        AsyncValue<::AsyncValueTestEnum> value{ ::DEFAULT_ENUM_VALUE };
        value.set(::SET_ENUM_VALUE);

        CHECK(value.ready());
        CHECK(value.get() == ::SET_ENUM_VALUE);
    }
}

} // namespace sfa::testing
