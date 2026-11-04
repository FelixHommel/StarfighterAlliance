#ifndef SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_JSON_FILE_HPP
#define SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_JSON_FILE_HPP

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>

namespace sfa::testing
{

class TemporaryDirectory
{
public:
    explicit TemporaryDirectory(std::string_view name) : m_path{ createUniqueDirectory(name) } {}
    ~TemporaryDirectory() = default;

    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;
    TemporaryDirectory(TemporaryDirectory&&) = delete;
    TemporaryDirectory& operator=(TemporaryDirectory&&) = delete;

    [[nodiscard]] const std::filesystem::path& path() const noexcept { return m_path; }

private:
    std::filesystem::path m_path;

    static std::string sanitizeName(std::string_view name)
    {
        std::string result;
        result.reserve(std::min(name.size(), std::size_t{ 48 }));

        for(const unsigned char c : name)
        {
            if(result.size() >= 48)
                break;

            result += static_cast<bool>(std::isalnum(c)) || c == '-' || c == '_' ? static_cast<char>(c) : '_';
        }

        return result.empty() ? "temporary" : result;
    }

    static std::filesystem::path createUniqueDirectory(std::string_view name)
    {
        static std::atomic<std::uint64_t> counter{};

        const auto basePath{ std::filesystem::temp_directory_path() };
        const auto prefix{ sanitizeName(name) };

        std::random_device randomDevice;

        for(;;)
        {
            const auto nonce{ (static_cast<std::uint64_t>(randomDevice()) << 32)
                              ^ static_cast<std::uint64_t>(randomDevice()) };

            std::ostringstream suffix;
            suffix << std::hex << nonce << '_' << counter.fetch_add(1);

            const auto path{ basePath / (prefix + '_' + suffix.str()) };

            if(!std::filesystem::create_directory(path))
                continue;

            try
            {
                std::filesystem::permissions(
                    path, std::filesystem::perms::owner_all, std::filesystem::perm_options::replace
                );
            }
            catch(...)
            {
                std::error_code error;
                std::filesystem::remove_all(path, error);

                throw;
            }

            return path;
        }
    }
};

enum class JsonFileType : std::uint8_t
{
    ValidJson,
    ValidSchema,
    MalformedJson,
    MalformedSchema,
    NonSchemaConformJson,
};


template<JsonFileType... FileTypes>
consteval bool areUniqueFileTypes()
{
    constexpr std::array<JsonFileType, sizeof...(FileTypes)> types{ FileTypes... };

    for(std::size_t i{ 0 }; i < types.size(); ++i)
    {
        for(std::size_t j{ i + 1 }; j < types.size(); ++j)
        {
            if(types[i] == types[j])
                return false;
        }
    }

    return true;
}

template<JsonFileType... FileTypes>
    requires(areUniqueFileTypes<FileTypes...>())
class TemporaryJsonFile
{
public:
    explicit TemporaryJsonFile(const ::testing::TestInfo* info)
        : TemporaryJsonFile(std::string(info->test_suite_name()) + "_" + std::string(info->name()))
    {}

    explicit TemporaryJsonFile(const ::testing::TestSuite* info) : TemporaryJsonFile(info->name()) {}

    explicit TemporaryJsonFile(std::string_view dirName) : m_tempDir{ dirName } { (createFile(FileTypes), ...); }

    ~TemporaryJsonFile() = default;

    TemporaryJsonFile(const TemporaryJsonFile&) = delete;
    TemporaryJsonFile& operator=(const TemporaryJsonFile&) = delete;
    TemporaryJsonFile(TemporaryJsonFile&&) = delete;
    TemporaryJsonFile& operator=(TemporaryJsonFile&&) = delete;

    [[nodiscard]] std::filesystem::path filePath(JsonFileType fileType) const
    {
        return m_tempDir.path() / (jsonFileTypeToString(fileType).data() + std::string(".json"));
    }

    [[nodiscard]] const std::filesystem::path& directoryPath() const noexcept { return m_tempDir.path(); }

private:
    TemporaryDirectory m_tempDir;

    static constexpr auto VALID_JSON{ R"({ "key1": "value", "key2": 1, "key3": 2.3 })" };

    void createFile(JsonFileType fileType) const
    {
        std::ofstream output;
        output.exceptions(std::ios::badbit | std::ios::failbit);

        output.open(filePath(fileType), std::ios::binary);
        output << jsonFileTypeToContent(fileType);
        output.close();
    }

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
            return {};
        }
    }

    static constexpr std::string_view jsonFileTypeToContent(JsonFileType fileType)
    {
        switch(fileType)
        {
            using enum JsonFileType;

        case ValidJson:
            return VALID_JSON;
        case ValidSchema:
            return "valid_schema";
        case MalformedJson:
            return "malformed_json";
        case MalformedSchema:
            return "malformed_schema";
        case NonSchemaConformJson:
            return "non_schema_conform_json";
        default:
            return {};
        }
    }
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_JSON_FILE_HPP

