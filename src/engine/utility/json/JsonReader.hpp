#ifndef SFA_SRC_ENGINE_UTILITY_JSON_JSON_READER_HPP
#define SFA_SRC_ENGINE_UTILITY_JSON_JSON_READER_HPP

#include "utility/json/JsonError.hpp"
#include "utility/json/ValidationError.hpp"
#include "utility/json/ValidationResult.hpp"

#include <nlohmann/json.hpp>

#include <expected>
#include <filesystem>
#include <optional>

namespace sfa::json
{

namespace validation
{

[[nodiscard]] std::expected<ValidationResult, ValidationError> validate(
    const nlohmann::json& doc, const std::filesystem::path& schemaPath
);

} // namespace validation

[[nodiscard]] std::expected<nlohmann::json, JsonError> parseJson(
    const std::filesystem::path& path, const std::optional<std::filesystem::path>& schema = std::nullopt
);

} // namespace sfa::json

#endif // !SFA_SRC_ENGINE_UTILITY_JSON_JSON_READER_HPP

