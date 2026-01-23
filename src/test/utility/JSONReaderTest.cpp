#include "gtest/gtest.h"

#include "core/JSONReader.hpp"

#include <memory>
#include <string>

namespace sfa::testing
{

/// \brief Test the \ref JSONReader class.
///
/// \author Felix Hommel
/// \date 1/23/2026
class JSONReaderTest : public ::testing::Test
{
public:
    JSONReaderTest() = default;
    ~JSONReaderTest() override = default;

    JSONReaderTest(const JSONReaderTest&) = delete;
    JSONReaderTest(JSONReaderTest&&) = delete;
    JSONReaderTest& operator=(const JSONReaderTest&) = delete;
    JSONReaderTest& operator=(JSONReaderTest&&) = delete;

    void SetUp() override
    {
        reader = std::make_unique<JSONReader>();
    }

protected:
    static constexpr auto NOT_EXISTING_JSON_PATH{ "nonexistent.file" };
    static constexpr auto VALID_JSON_TEST_RESOURCE_PATH{ SFA_ROOT "src/test/testResources/json/valid.json" };
    static constexpr auto INVALID_JSON_TEST_RESOURCE_PATH{ SFA_ROOT "src/test/testResources/json/invalid.json" };

    std::unique_ptr<JSONReader> reader;
};

/// \brief Test that the \ref JSONReader can read a single value from a JSON file.
TEST_F(JSONReaderTest, ReadSingleValue)
{
    ASSERT_TRUE(reader->open(VALID_JSON_TEST_RESOURCE_PATH));

    const auto val{ reader->get<std::string>("key1") };
    ASSERT_TRUE(val.has_value());
    EXPECT_STREQ("value1", val.value_or("").c_str());
}

/// \brief Test that the \ref JSONReader can read multiple values from a JSON file.
TEST_F(JSONReaderTest, ReadAllValues)
{
    ASSERT_TRUE(reader->open(VALID_JSON_TEST_RESOURCE_PATH));

    const auto val{ reader->get<std::string>("key1") };
    ASSERT_TRUE(val.has_value());
    EXPECT_STREQ("value1", val.value_or("").c_str());

    const auto val2{ reader->get<unsigned int>("key2") };
    ASSERT_TRUE(val2.has_value());
    EXPECT_EQ(2, val2.value_or(0));

    const auto val3{ reader->get<float>("key3") };
    ASSERT_TRUE(val3.has_value());
    EXPECT_FLOAT_EQ(3.3f, val3.value_or(0.f));
}

/// \brief Test that the \ref JSONReader returns a std::nullopt when a key does not exist in a JSON file.
TEST_F(JSONReaderTest, ReadNonExistingValue)
{
    ASSERT_TRUE(reader->open(VALID_JSON_TEST_RESOURCE_PATH));

    const auto val1{ reader->get<std::string>("notAKey") };
    EXPECT_FALSE(val1.has_value());
}

/// \brief Test that the open() method of \ref JSONReader returns false when the provided filepath does not lead to a file.
TEST_F(JSONReaderTest, OpenNotExistingFile)
{
    EXPECT_FALSE(reader->open(NOT_EXISTING_JSON_PATH));
}

/// \brief Test that the open() method of \ref JSONReader returns false when the provided filepath is not a valid JSON file.
TEST_F(JSONReaderTest, ParseInvalidJSONFile)
{
    EXPECT_FALSE(reader->open(INVALID_JSON_TEST_RESOURCE_PATH));
}

} // namespace sfa::testing
