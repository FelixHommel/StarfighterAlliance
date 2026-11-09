#ifndef SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_JSON_FILE_HPP
#define SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_JSON_FILE_HPP

#include "testUtility/TemporaryDirectory.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <string>
#include <string_view>

namespace sfa::testing
{

/// \brief Enum that represents the available JSON files.
///
/// \author Felix Hommel
/// \date 11/04/2026
enum class JsonFileType : std::uint8_t
{
    ValidJson,
    ValidSchema,
    MalformedJson,
    MalformedSchema,
    NonSchemaConformJson,
};

/// brief Convert a \ref JsonFileType to its \ref std::string representation.
///
/// \param fileType The \ref JsonFileType that is converted
///
/// \returns string representation of \p fileType
static constexpr std::string_view jsonFileTypeToString(JsonFileType fileType)
{
    switch(fileType)
    {
        using enum JsonFileType;

    case ValidJson:
        return "valid_json";
    case ValidSchema:
        return "valid_schema";
    case MalformedJson:
        return "malformed_json";
    case MalformedSchema:
        return "malformed_schema";
    case NonSchemaConformJson:
        return "non_schema_conform_json";
    default:
        return "";
    }
}

namespace detail
{

/// \brief Check if a template parameter pack consists of only unique parameters.
///
/// \tparam FileTypes parameter pack of \ref JsonFileType
///
/// \returns *true* if \p FileTypes are unique, *false* otherwise
template<JsonFileType... FileTypes>
consteval bool areUniqueFileTypes()
{
    constexpr std::array<JsonFileType, sizeof...(FileTypes)> types{ FileTypes... };

    for(std::size_t i{ 0 }; i < types.size(); ++i)
    {
        for(std::size_t j{ i + 1 }; j < types.size(); ++j)
        {
            if(types.at(i) == types.at(j))
                return false;
        }
    }

    return true;
}

} // namespace detail

/// \brief Create test JSON files in a temporary directory.
///
/// \tparam FileTypes Parameter pack of \ref JsonFileType. They are required to be unique.
///
/// \author Felix Hommel
/// \date 11/04/2026
template<JsonFileType... FileTypes>
    requires(detail::areUniqueFileTypes<FileTypes...>())
class TemporaryJsonFile
{
public:
    /// \brief Create a new temporary JSON directory with a given name.
    explicit TemporaryJsonFile(std::string_view dirName) : m_tempDir{ dirName } { (createFile(FileTypes), ...); }

    ~TemporaryJsonFile() = default;

    TemporaryJsonFile(const TemporaryJsonFile&) = delete;
    TemporaryJsonFile& operator=(const TemporaryJsonFile&) = delete;
    TemporaryJsonFile(TemporaryJsonFile&&) = delete;
    TemporaryJsonFile& operator=(TemporaryJsonFile&&) = delete;

    /// \brief Get the path to a specific JSON file type.
    ///
    /// \param fileType The \ref JsonFileType for which the path is returned
    ///
    /// \returns The path to the JSON file with \p fileType
    ///
    /// \throws \ref std::invalid_argument if \p fileType is not specified in \p FileTypes
    [[nodiscard]] std::filesystem::path filePath(JsonFileType fileType) const
    {
        if(!((fileType == FileTypes) || ...))
            throw std::invalid_argument("JSON file type was not requested");

        return m_tempDir.path() / (jsonFileTypeToString(fileType).data() + std::string(".json"));
    }

    [[nodiscard]] const std::filesystem::path& directoryPath() const noexcept { return m_tempDir.path(); }

private:
    TemporaryDirectory m_tempDir;

    static constexpr auto VALID_JSON{ R"({ "key1": "value", "key2": 1, "key3": 2.3 })" };
    static constexpr auto VALID_SCHEMA{
        R"({ "$schema": "https://json-schema.org/draft/2020-12/schema", "type": "object", "properties": { "key1": { "description": "First property", "type": "string" }, "key1": { "description": "First property", "type": "string" }, "key2": { "description": "Second property", "type": "integer" }, "key3": { "description": "Third property", "type": "number" } } })"
    };
    static constexpr auto MALFORMED_JSON{ R"({ "key1" "value", "key2": 1, "key3": 2.3 })" };
    static constexpr auto MALFORMED_SCHEMA{
        R"({ "$schema": "https://json-schema.org/draft/2020-12/schema", "type": "object", "properties": { "key1": { "description" "First property", "type": "string" }, "key1": { "description": "First property", "type": "string" }, "key2": { "description": "Second property", "type": "integer" }, "key3": { "description": "Third property", "type": "number" } } })"
    };
    static constexpr auto NON_SCHEMA_CONFORM_JSON{ R"({ "key1": "value", "key2": 1.0, "key3": 2.3 })" };

    /// \brief Create a JSON file with the specified content.
    ///
    /// \param fileType The \ref JsonFileType that specifies what the file contains
    void createFile(JsonFileType fileType) const
    {
        std::ofstream output;
        output.exceptions(std::ios::badbit | std::ios::failbit);

        output.open(filePath(fileType), std::ios::binary);
        output << jsonFileTypeToContent(fileType);
        output.close();
    }

    /// \brief mapping function assigning each \ref JsonFileType to the specific content of the file.
    ///
    /// \param fileType The \ref JsonFileType whose content is queried.
    ///
    /// \returns The content of \p fileType in string format
    static constexpr std::string_view jsonFileTypeToContent(JsonFileType fileType)
    {
        switch(fileType)
        {
            using enum JsonFileType;

        case ValidJson:
            return VALID_JSON;
        case ValidSchema:
            return VALID_SCHEMA;
        case MalformedJson:
            return MALFORMED_JSON;
        case MalformedSchema:
            return MALFORMED_SCHEMA;
        case NonSchemaConformJson:
            return NON_SCHEMA_CONFORM_JSON;
        default:
            return "{}";
        }
    }
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_JSON_FILE_HPP

