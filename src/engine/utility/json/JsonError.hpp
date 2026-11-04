#ifndef SFA_SRC_ENGINE_UTILITY_JSON_JSON_ERROR_HPP
#define SFA_SRC_ENGINE_UTILITY_JSON_JSON_ERROR_HPP

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <utility>

namespace sfa::json
{

class JsonError
{
public:
    enum class Reason : std::uint8_t
    {
        FileNotFound,
        ParsingFailed,
        SchemaError,
        ValidationFailed
    };

    JsonError(Reason reason, const std::filesystem::path& errorPath, std::string message = {})
        : m_reason{ reason }, m_errorPath{ errorPath }, m_message{ std::move(message) }
    {}
    JsonError(
        Reason reason,
        const std::filesystem::path& errorPath,
        const std::filesystem::path& schemaPath,
        std::string message = {}
    )
        : m_reason{ reason }, m_errorPath{ errorPath }, m_schemaPath{ schemaPath }, m_message{ std::move(message) }
    {}
    ~JsonError() = default;

    JsonError(const JsonError&) = default;
    JsonError& operator=(const JsonError&) = default;
    JsonError(JsonError&&) = default;
    JsonError& operator=(JsonError&&) = default;

    [[nodiscard]] Reason reason() const noexcept { return m_reason; }
    [[nodiscard]] const std::filesystem::path& path() const noexcept { return m_errorPath; }
    [[nodiscard]] const std::filesystem::path& schemaPath() const noexcept { return m_schemaPath; }
    [[nodiscard]] std::string_view message() const noexcept { return m_message; }

private:
    Reason m_reason;
    std::filesystem::path m_errorPath;
    std::filesystem::path m_schemaPath;
    std::string m_message;
};

} // namespace sfa::json

#endif // !SFA_SRC_ENGINE_UTILITY_JSON_JSON_ERROR_HPP

