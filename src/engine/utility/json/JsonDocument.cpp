#include "JsonDocument.hpp"

#include <nlohmann/json.hpp>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/utils/nlohmann_json_utils.hpp>
#include <valijson/validator.hpp>

#include <filesystem>
#include <fstream>

namespace sfa
{

JsonDocument::JsonDocument(const std::filesystem::path& filepath)
{
    load(filepath);
}

void JsonDocument::load(const std::filesystem::path& filepath)
{
    std::ifstream f(filepath);
    m_json = nlohmann::json::parse(f);
}

bool JsonDocument::validate(const std::filesystem::path& schemaPath)
{
    if(m_json == nullptr)
        return false;

    valijson::Validator validator;
    nlohmann::json schemaDoc;

    if(!valijson::utils::loadDocument(schemaPath.string(), schemaDoc))
        return false;

    valijson::Schema schema;
    valijson::SchemaParser parser;
    valijson::adapters::NlohmannJsonAdapter schemaAdapter{ schemaDoc };
    parser.populateSchema(schemaAdapter, schema);

    valijson::adapters::NlohmannJsonAdapter validationDoc{ m_json };
    return validator.validate(schema, validationDoc, nullptr);
}

} // namespace sfa
