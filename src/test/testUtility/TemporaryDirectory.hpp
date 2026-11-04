#ifndef SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_DIRECTORY_HPP
#define SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_DIRECTORY_HPP

#include <filesystem>
#include <string_view>

namespace sfa::testing
{

/// A RAII wrapper for a temporary directory. Can be used for tests that need file-based resources.
///
/// \author Felix Hommel
/// \date 11/04/2026
class TemporaryDirectory
{
public:
    /// \brief Create a new temporary directory with a given name.
    ///
    /// \param name The name of the directory
    explicit TemporaryDirectory(std::string_view name);
    ~TemporaryDirectory() noexcept;

    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;
    TemporaryDirectory(TemporaryDirectory&&) = delete;
    TemporaryDirectory& operator=(TemporaryDirectory&&) = delete;

    [[nodiscard]] const std::filesystem::path& path() const noexcept { return m_path; }

private:
    std::filesystem::path m_path;
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_TEST_UTILITY_TEMPORARY_DIRECTORY_HPP
