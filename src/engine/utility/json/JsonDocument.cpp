#include "JsonDocument.hpp"

#include <nlohmann/json_fwd.hpp>

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

} // namespace sfa
