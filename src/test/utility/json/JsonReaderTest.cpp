#include "utility/json/JsonReader.hpp"
#include "testUtility/TemporaryJsonFile.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <random>
#include <string>

namespace sfa::testing
{

/// \brief Test that reading a normal JSON file works as expected.
///
/// Reading a syntactically correct JSON file should return a \ref nlohmann::json object wrapped in a \ref std::expected.
TEST(JsonReaderTest, ReadingValidFileFromDisk)
{
    TemporaryJsonFile<JsonFileType::ValidJson> json{ ::testing::UnitTest::GetInstance()->current_test_info() };

    const auto parseResult{ json::parseJson(json.filePath(JsonFileType::ValidJson)) };

    EXPECT_TRUE(parseResult.has_value());
}

/// \brief Test that reading a non-existing file works as expected.
///
/// Reading a non-existing file should not throw or cause any other fatal issue, instead it should simply return a
/// \ref std::unexpected value containing a \ref JsonError object that describes the cause of the error.
TEST(JsonReaderTest, AttemptReadingFromNonExistingFile)
{
    std::random_device random;
    const auto path{ std::filesystem::temp_directory_path() / std::to_string(random()) };

    const auto parseResult{ json::parseJson(path) };

    ASSERT_FALSE(parseResult.has_value());

    const auto error{ parseResult.error() };

    EXPECT_EQ(error.reason(), json::JsonError::Reason::FileNotFound);
    EXPECT_EQ(error.path(), path);
}

/// \brief Test that reading a malformed JSON file works as expected.
///
/// Reading a malformed JSON file should not throw or cause other fatal issues, instead it should simply return a
/// \ref std::unexpected value containing a \ref JsonError object that describes the cause of the error.
TEST(JsonReaderTest, ReadingMalformedJsonFileFromDisk)
{
    TemporaryJsonFile<JsonFileType::MalformedJson> json{ ::testing::UnitTest::GetInstance()->current_test_info() };
    const auto& path{ json.filePath(JsonFileType::MalformedJson) };

    const auto parseResult{ json::parseJson(path) };

    ASSERT_FALSE(parseResult.has_value());

    const auto error{ parseResult.error() };

    EXPECT_EQ(error.reason(), json::JsonError::Reason::ParsingFailed);
    EXPECT_EQ(error.path(), path);
}

/// \brief Test that reading a valid and schema conform JSON file with using validation works as expected.
///
/// Reading a valid JSON file that conforms to the schema should result in a \ref nlohmann::json object wrapped inside
/// a \ref std::expected being returned.
TEST(JsonReaderTest, ReadingValidFileFromDiskWithValidation)
{
    TemporaryJsonFile<JsonFileType::ValidJson, JsonFileType::ValidSchema> json{
        ::testing::UnitTest::GetInstance()->current_test_info()
    };
    const auto& jsonPath{ json.filePath(JsonFileType::ValidJson) };
    const auto& schemaPath{ json.filePath(JsonFileType::ValidSchema) };

    const auto parseResult{ json::parseJson(jsonPath, schemaPath) };

    EXPECT_TRUE(parseResult.has_value());
}

/// \brief Test that using a malformed JSON schema file works as expected.
///
/// When the schema file itself is malformed (i.e., contains syntax errors) there should not be any exceptions thrown
/// or other fatal errors to be had, instead a \ref std::unexpected object containing a \ref JsonError describing the
/// problem is returned.
TEST(JsonReaderTest, AttemptUsingMalformedSchema)
{
    TemporaryJsonFile<JsonFileType::ValidJson, JsonFileType::MalformedSchema> json{
        ::testing::UnitTest::GetInstance()->current_test_info()
    };
    const auto& jsonPath{ json.filePath(JsonFileType::ValidJson) };
    const auto& schemaPath{ json.filePath(JsonFileType::MalformedSchema) };

    const auto parseResult{ json::parseJson(jsonPath, schemaPath) };

    ASSERT_FALSE(parseResult.has_value());

    const auto error{ parseResult.error() };

    EXPECT_EQ(error.reason(), json::JsonError::Reason::SchemaError);
    EXPECT_EQ(error.path(), jsonPath);
    EXPECT_EQ(error.schemaPath(), schemaPath);
}

/// \brief Test that reading a non-schema conform JSON file with usage of validation works as expected.
///
/// When validation is used and the JSON file that is read is found to be not conform to the schema there should not be
/// any exceptions that are thrown nor any other fatal errors that occur. Instead, the system should catch the
/// nonconformity and report the schema issues to the user and return a \ref std::unexpected value containing a
/// \ref JsonError object that describes the issue.
TEST(JsonReaderTest, AttemptReadingNonSchemaConformFromDiskWithValidation)
{
    TemporaryJsonFile<JsonFileType::NonSchemaConformJson, JsonFileType::ValidSchema> json{
        ::testing::UnitTest::GetInstance()->current_test_info()
    };
    const auto& jsonPath{ json.filePath(JsonFileType::NonSchemaConformJson) };
    const auto& schemaPath{ json.filePath(JsonFileType::ValidSchema) };

    const auto parseResult{ json::parseJson(jsonPath, schemaPath) };

    ASSERT_FALSE(parseResult.has_value());

    const auto error{ parseResult.error() };

    EXPECT_EQ(error.reason(), json::JsonError::Reason::ValidationFailed);
    EXPECT_EQ(error.path(), jsonPath);
    EXPECT_TRUE(false);
}

} // namespace sfa::testing

