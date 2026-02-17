#ifndef SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_ERROR_HPP
#define SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_ERROR_HPP

#include <cstdint>
#include <filesystem>
#include <string>
#include <utility>

namespace sfa
{

/// \brief Simple struct to specify why and what went wrong while trying to load a resource.
///
/// \author Felix Hommel
/// \date 2/10/2026
struct ResourceError
{
    /// \brief What kind of error occured.
    enum class Type : std::uint8_t
    {
        FileNotFound,
        InvalidFormat,
        ParseError,
        GLError,
        Unknown
    };

    Type type;
    std::string message;
    std::filesystem::path filepath;

    static ResourceError fileNotFound(const std::filesystem::path& path)
    {
        return { .type = Type::FileNotFound, .message = "File was not found on the disk", .filepath = path };
    }

    static ResourceError invalidFormat(const std::filesystem::path& path, std::string message)
    {
        return { .type = Type::InvalidFormat, .message = std::move(message), .filepath = path };
    }

    static ResourceError parseError(const std::filesystem::path& path, std::string message)
    {
        return { .type = Type::ParseError, .message = std::move(message), .filepath = path };
    }
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_ERROR_HPP

