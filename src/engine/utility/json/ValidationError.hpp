#ifndef SFA_SRC_ENGINE_UTILITY_JSON_VALIDATION_ERROR_HPP
#define SFA_SRC_ENGINE_UTILITY_JSON_VALIDATION_ERROR_HPP

#include <filesystem>

namespace sfa::json::validation
{

class ValidationError
{
public:
    explicit ValidationError(const std::filesystem::path& schemaPath) : m_schema{ schemaPath } {}

    [[nodiscard]] const std::filesystem::path& schemaPath() const noexcept { return m_schema; }

private:
    std::filesystem::path m_schema;
};

} // namespace sfa::json::validation

#endif // !SFA_SRC_ENGINE_UTILITY_JSON_VALIDATION_ERROR_HPP

