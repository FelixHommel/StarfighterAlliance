#ifndef SFA_SRC_ENGINE_UTILITY_JSON_JSON_DOCUMENT_HPP
#define SFA_SRC_ENGINE_UTILITY_JSON_JSON_DOCUMENT_HPP

#include "nlohmann/json.hpp"

#include <filesystem>
#include <string_view>

namespace sfa
{

/// \brief Wrapper class to make JSON document access more convenient
///
/// \author Felix Hommel
/// \date 5/23/2026
class JsonDocument
{
public:
    /// \brief Construct an empty \ref JsonDocument
    JsonDocument() = default;
    /// \brief Construct a \ref JsonDocument and load a document from file
    ///
    /// \param filepath path to the JSON file on the disk
    explicit JsonDocument(const std::filesystem::path& filepath);
    ~JsonDocument() = default;

    JsonDocument(const JsonDocument&) = delete;
    JsonDocument& operator=(const JsonDocument&) = delete;
    JsonDocument(JsonDocument&&) = delete;
    JsonDocument& operator=(JsonDocument&&) = delete;

    /// \brief Retrieve a value from the JSON document
    ///
    /// \tparam T expected type of the value at \p key
    /// \param key name of the filed of which the value is retrieved
    template<typename T>
    T get(std::string_view key)
    {
        return m_json[key].get<T>();
    }

    /// \brief Load a JSON document file
    ///
    /// \param filepath path to the JSON file
    void load(const std::filesystem::path& filepath);

    /// \brief Validate the document against a schema
    ///
    /// \param schemaPath path to the schema file to validate against
    ///
    /// \returns \p true if the document validated against the schema, \p false otherwise
    bool validate(const std::filesystem::path& schemaPath);

private:
    nlohmann::json m_json{ nullptr };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_JSON_JSON_DOCUMENT_HPP

