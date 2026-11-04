#include "TemporaryDirectory.hpp"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <ios>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>

namespace
{

/// \brief Sanitize a user given directory name.
///
/// Allowed are alphanumeric characters as well as '-' and '_'. Everything else is replaced by '_'.
///
/// \param name The user provided name
///
/// \returns sanitized version of \p name
std::string sanitizeName(std::string_view name)
{
    static constexpr std::size_t MAX_NAME_LENGTH{ 48 };

    std::string result;
    result.reserve(std::min(name.size(), MAX_NAME_LENGTH));

    for(const auto& c : name)
    {
        if(result.size() >= MAX_NAME_LENGTH)
            break;

        result += (std::isalnum(c) != 0 || c == '-' || c == '_') ? static_cast<char>(c) : '_';
    }

    return result.empty() ? "temporary" : result;
}

/// \brief Create a new unique directory within the temporary directory.
///
/// Creates a new temporary directory and gives it a unique name.
///
/// \param name The user-given name for the directory
///
/// \returns The \ref std::filesystem::path to the new temporary directory
///
/// \throws If it wasn't possible to change the permissions of the temporary file
std::filesystem::path createUniqueDirectory(std::string_view name)
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

} // namespace

namespace sfa::testing
{

TemporaryDirectory::TemporaryDirectory(std::string_view name) : m_path{ ::createUniqueDirectory(name) } {}

TemporaryDirectory::~TemporaryDirectory() noexcept
{
    std::error_code error;
    std::filesystem::remove_all(m_path, error);
}

} // namespace sfa::testing

