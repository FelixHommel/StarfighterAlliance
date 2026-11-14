#include "utility/json/JsonReader.hpp"
#include "testUtility/TemporaryJsonFile.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/interfaces/catch_interfaces_capture.hpp>
#include <catch2/internal/catch_context.hpp>

#include <filesystem>
#include <random>
#include <string>

namespace sfa::testing
{

TEST_CASE("JsonReaderTest", "[unit][utility][filesystem]")
{
    /// \brief Test that reading a normal JSON file works as expected.
    ///
    /// Reading a syntactically correct JSON file should return a \ref nlohmann::json object wrapped in a \ref std::expected.
    SECTION("Reading valid file from disk")
    {
        TemporaryJsonFile<JsonFileType::ValidJson> json{ Catch::getResultCapture().getCurrentTestName() };

        const auto parseResult{ json::parseJson(json.filePath(JsonFileType::ValidJson)) };

        CHECK(parseResult.has_value());
    }

    /// \brief Test that reading a non-existing file works as expected.
    ///
    /// Reading a non-existing file should not throw or cause any other fatal issue, instead it should simply return a
    /// \ref std::unexpected value containing a \ref JsonError object that describes the cause of the error.
    SECTION("Attempt reading from non existing file")
    {
        std::random_device random;
        const auto path{ std::filesystem::temp_directory_path() / std::to_string(random()) };

        const auto parseResult{ json::parseJson(path) };

        REQUIRE(!parseResult.has_value());

        const auto error{ parseResult.error() };

        CHECK(error.reason() == json::JsonError::Reason::FileNotFound);
        CHECK(error.path() == path);
    }

    /// \brief Test that reading a malformed JSON file works as expected.
    ///
    /// Reading a malformed JSON file should not throw or cause other fatal issues, instead it should simply return a
    /// \ref std::unexpected value containing a \ref JsonError object that describes the cause of the error.
    SECTION("Reading malformed JSON file from disk")
    {
        TemporaryJsonFile<JsonFileType::MalformedJson> json{ Catch::getResultCapture().getCurrentTestName() };
        const auto& path{ json.filePath(JsonFileType::MalformedJson) };

        const auto parseResult{ json::parseJson(path) };

        REQUIRE(!parseResult.has_value());

        const auto error{ parseResult.error() };

        CHECK(error.reason() == json::JsonError::Reason::ParsingFailed);
        CHECK(error.path() == path);
    }

    /// \brief Test that reading a valid and schema conform JSON file with using validation works as expected.
    ///
    /// Reading a valid JSON file that conforms to the schema should result in a \ref nlohmann::json object wrapped inside
    /// a \ref std::expected being returned.
    SECTION("Reading valid file from disk with validation")
    {
        TemporaryJsonFile<JsonFileType::ValidJson, JsonFileType::ValidSchema> json{
            Catch::getResultCapture().getCurrentTestName()
        };
        const auto& jsonPath{ json.filePath(JsonFileType::ValidJson) };
        const auto& schemaPath{ json.filePath(JsonFileType::ValidSchema) };

        const auto parseResult{ json::parseJson(jsonPath, schemaPath) };

        CHECK(parseResult.has_value());
    }

    /// \brief Test that using a malformed JSON schema file works as expected.
    ///
    /// When the schema file itself is malformed (i.e., contains syntax errors) there should not be any exceptions thrown
    /// or other fatal errors to be had, instead a \ref std::unexpected object containing a \ref JsonError describing the
    /// problem is returned.
    SECTION("Attempt using malformed schema")
    {
        TemporaryJsonFile<JsonFileType::ValidJson, JsonFileType::MalformedSchema> json{
            Catch::getResultCapture().getCurrentTestName()
        };
        const auto& jsonPath{ json.filePath(JsonFileType::ValidJson) };
        const auto& schemaPath{ json.filePath(JsonFileType::MalformedSchema) };

        const auto parseResult{ json::parseJson(jsonPath, schemaPath) };

        REQUIRE(!parseResult.has_value());

        const auto error{ parseResult.error() };

        CHECK(error.reason() == json::JsonError::Reason::SchemaError);
        CHECK(error.path() == jsonPath);
        CHECK(error.schemaPath() == schemaPath);
    }

    /// \brief Test that reading a non-schema conform JSON file with usage of validation works as expected.
    ///
    /// When validation is used and the JSON file that is read is found to be not conform to the schema there should not be
    /// any exceptions that are thrown nor any other fatal errors that occur. Instead, the system should catch the
    /// nonconformity and report the schema issues to the user and return a \ref std::unexpected value containing a
    /// \ref JsonError object that describes the issue.
    SECTION("Attempt reading non schema conform from disk with validation")
    {
        TemporaryJsonFile<JsonFileType::NonSchemaConformJson, JsonFileType::ValidSchema> json{
            Catch::getResultCapture().getCurrentTestName()
        };
        const auto& jsonPath{ json.filePath(JsonFileType::NonSchemaConformJson) };
        const auto& schemaPath{ json.filePath(JsonFileType::ValidSchema) };

        const auto parseResult{ json::parseJson(jsonPath, schemaPath) };

        REQUIRE(!parseResult.has_value());

        const auto error{ parseResult.error() };

        CHECK(error.reason() == json::JsonError::Reason::ValidationFailed);
        CHECK(error.path() == jsonPath);
    }
}

} // namespace sfa::testing

