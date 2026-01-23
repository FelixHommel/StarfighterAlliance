#include "gtest/gtest.h"

#include "MockTexture.hpp"

#include "utility/JSONReader.hpp"

/** JSON TEST */
TEST(JSON, ReadSingleValue)
{
    JSONReader r;
    r.open("resources/config/x_wing.json");

    auto val{ r.get<unsigned int>("lives") };
    ASSERT_TRUE(val.has_value());
    ASSERT_EQ(3, val.value());
}

TEST(JSON, ReadAllValues)
{
    JSONReader r;
    r.open("resources/config/x_wing.json");

    auto val1{ r.get<unsigned int>("lives") };
    ASSERT_TRUE(val1.has_value());
    ASSERT_EQ(3, val1.value());

    auto val2{ r.get<unsigned int>("speed") };
    ASSERT_TRUE(val2.has_value());
    ASSERT_EQ(3, val2.value());

    auto val3{ r.get<unsigned int>("shotPower") };
    ASSERT_TRUE(val3.has_value());
    ASSERT_EQ(10, val3.value());
}

TEST(JSON, ReadNonExistingValue)
{
    JSONReader r;
    r.open("resources/config/x_wing.json");

    auto val1{ r.get<unsigned int>("live") };
    ASSERT_FALSE(val1.has_value());
}

TEST(JSON, AttemptOpenNonExistingFile)
{
    JSONReader r;
    ASSERT_FALSE(r.open("resources/config/z_wing.json"));
}

TEST(JSON, AttemptOpenInvalidFile)
{
    JSONReader r;
    ASSERT_FALSE(r.open("test/resources/invalid.json"));
}
