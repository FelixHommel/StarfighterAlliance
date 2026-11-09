#ifndef SFA_TEST_TEST_UTILITY_RESOURCE_GENERATOR_HPP
#define SFA_TEST_TEST_UTILITY_RESOURCE_GENERATOR_HPP

#include "testUtility/TemporaryDirectory.hpp"

#include <stb_image_write.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <string_view>

namespace sfa::testing
{

/// \brief Utility for generating temporary resources that can be used within a test.
///
/// Creates a temporary directory and then writes test resources to the directory.
///
/// \author Felix Hommel
/// \date 2/17/2026
class ResourceGenerator
{
public:
    explicit ResourceGenerator(std::string_view dirName) : m_tempDir{ dirName }
    {
        createTestShader();
        createTestTexture();
    }

    ~ResourceGenerator() = default;

    ResourceGenerator(const ResourceGenerator&) = delete;
    ResourceGenerator& operator=(const ResourceGenerator&) = delete;
    ResourceGenerator(ResourceGenerator&&) noexcept = delete;
    ResourceGenerator& operator=(ResourceGenerator&&) noexcept = delete;

    [[nodiscard]] std::filesystem::path vertPath() const noexcept { return m_tempDir.path() / "test.vert"; }
    [[nodiscard]] std::filesystem::path fragPath() const noexcept { return m_tempDir.path() / "test.frag"; }
    [[nodiscard]] std::filesystem::path geomPath() const noexcept { return m_tempDir.path() / "test.geom"; }
    [[nodiscard]] std::filesystem::path texPath() const noexcept { return m_tempDir.path() / "test.png"; }
    [[nodiscard]] const std::filesystem::path& directoryPath() const noexcept { return m_tempDir.path(); }

private:
    TemporaryDirectory m_tempDir;

    void createTestShader() const
    {
        static constexpr auto SHADER_CODE{ R"(
            #version 330 core
            void main() {}
        )" };

        std::ofstream(vertPath()) << SHADER_CODE;
        std::ofstream(fragPath()) << SHADER_CODE;
        std::ofstream(geomPath()) << SHADER_CODE;
    }

    void createTestTexture() const
    {
        static constexpr auto IMAGE_WIDTH{ 1 };
        static constexpr auto IMAGE_HEIGHT{ 1 };
        static constexpr auto IMAGE_CHANNELS{ 4 };
        static constexpr std::array<unsigned char, 4> TEST_IMAGE_PIXELS{ 255, 255, 255, 255 };

        stbi_write_png(
            texPath().c_str(),
            IMAGE_WIDTH,
            IMAGE_HEIGHT,
            IMAGE_CHANNELS,
            TEST_IMAGE_PIXELS.data(),
            IMAGE_WIDTH * IMAGE_CHANNELS
        );
    }
};

} // namespace sfa::testing

#endif // !SFA_TEST_TEST_UTILITY_RESOURCE_GENERATOR_HPP

