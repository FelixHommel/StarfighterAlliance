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
        ValidationFailed
    };

    JsonError(Reason reason, const std::filesystem::path& errorPath, std::string message = {})
        : m_reason{ reason }, m_errorPath{ errorPath }, m_message{ std::move(message) }
    {}
    ~JsonError() = default;

    JsonError(const JsonError&) = default;
    JsonError& operator=(const JsonError&) = default;
    JsonError(JsonError&&) = default;
    JsonError& operator=(JsonError&&) = default;

    [[nodiscard]] Reason reason() const noexcept { return m_reason; }
    [[nodiscard]] const std::filesystem::path& path() const noexcept { return m_errorPath; }
    [[nodiscard]] std::string_view message() const noexcept { return m_message; }

private:
    Reason m_reason;
    std::filesystem::path m_errorPath;
    std::string m_message;
};

} // namespace sfa::json

#endif // !SFA_SRC_ENGINE_UTILITY_JSON_JSON_ERROR_HPP

