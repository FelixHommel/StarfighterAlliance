#include "JsonReader.hpp"

#include "utility/json/JsonError.hpp"
#include "utility/json/ValidationError.hpp"
#include "utility/json/ValidationResult.hpp"

#include <nlohmann/json_fwd.hpp>
#include <spdlog/spdlog.h>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/utils/nlohmann_json_utils.hpp>
#include <valijson/validation_results.hpp>
#include <valijson/validator.hpp>

#include <expected>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <numeric>
#include <optional>
#include <string>
#include <utility>

namespace sfa::json
{

namespace validation
{

[[nodiscard]] std::expected<ValidationResult, ValidationError> validate(
    const nlohmann::json& doc, const std::filesystem::path& schemaPath
)
{
    nlohmann::json schemaDoc{ nullptr };
    try
    {
        if(!valijson::utils::loadDocument(schemaPath, schemaDoc))
            return std::unexpected<ValidationError>({});
    }
    catch(...)
    {
        return std::unexpected<ValidationError>({});
    }

    valijson::Schema schema;
    valijson::SchemaParser parser;
    valijson::adapters::NlohmannJsonAdapter schemaAdapter{ schemaDoc };
    try
    {
        parser.populateSchema(schemaAdapter, schema);
    }
    catch(...)
    {
        return std::unexpected<ValidationError>({});
    }

    valijson::Validator validator{ valijson::Validator::kStrongTypes };
    valijson::ValidationResults results;
    valijson::adapters::NlohmannJsonAdapter targetAdapter{ doc };
    if(validator.validate(schema, targetAdapter, &results))
        return ValidationResult{ .isValid = true };

    return ValidationResult{ .isValid = false, .validationErrors = std::move(results) };
}

} // namespace validation

[[nodiscard]] std::expected<nlohmann::json, JsonError> parseJson(
    const std::filesystem::path& path, const std::optional<std::filesystem::path>& schema
)
{
    if(!std::filesystem::exists(path) || std::filesystem::is_directory(path))
        return std::unexpected<JsonError>({ JsonError::Reason::FileNotFound, path });

    std::ifstream fstream{ path };
    nlohmann::json doc{ nullptr };
    try
    {
        doc = nlohmann::json::parse(fstream);
    }
    catch(...)
    {
        return std::unexpected<JsonError>({ JsonError::Reason::ParsingFailed, path });
    }

    if(schema.has_value())
    {
        if(auto results{ validation::validate(doc, *schema) }; results.has_value())
        {
            if(!(*results).isValid)
            {
                spdlog::error("Validation of file '{}' failed, the following issues were found:", path.string());

                valijson::ValidationResults::Error error;
                unsigned int errorNumber{ 1u };
                while((*results).validationErrors.popError(error))
                {
                    spdlog::error(
                        "Error #{} @ {}\n\t- {}",
                        errorNumber++,
                        std::accumulate(
                            std::cbegin(error.context),
                            std::cend(error.context),
                            std::string(""),
                            [](std::string lhs, std::string rhs) { return std::move(lhs) + "::" + std::move(rhs); }
                        ),
                        error.description
                    );
                }

                return std::unexpected<JsonError>({ JsonError::Reason::ValidationFailed, path });
            }
        }
        else
            return std::unexpected<JsonError>({ JsonError::Reason::SchemaError, path, *schema });
    }

    return std::move(doc);
}

} // namespace sfa::json

