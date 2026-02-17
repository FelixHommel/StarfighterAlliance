#ifndef SFA_TEST_TEST_UTILITY_RESOURCE_GENERATOR_HPP
#define SFA_TEST_TEST_UTILITY_RESOURCE_GENERATOR_HPP

#include <gtest/gtest.h>
#include <stb_image_write.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <string>
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
    explicit ResourceGenerator(const ::testing::TestInfo* info)
        : ResourceGenerator(std::string(info->test_suite_name()) + "_" + std::string(info->name()))
    {}

    explicit ResourceGenerator(const ::testing::TestSuite* info) : ResourceGenerator(info->name()) {}

    explicit ResourceGenerator(std::string_view dirName)
        : m_tempDirPath{ std::filesystem::temp_directory_path() / dirName }
    {
        std::filesystem::create_directory(m_tempDirPath);

        createTestShader();
        createTestTexture();
    }

    ~ResourceGenerator() { std::filesystem::remove_all(m_tempDirPath); }

    ResourceGenerator(const ResourceGenerator&) = default;
    ResourceGenerator& operator=(const ResourceGenerator&) = default;
    ResourceGenerator(ResourceGenerator&&) noexcept = delete;
    ResourceGenerator& operator=(ResourceGenerator&&) noexcept = delete;

    [[nodiscard]] std::filesystem::path vertPath() const noexcept { return m_tempDirPath / "test.vert"; }
    [[nodiscard]] std::filesystem::path fragPath() const noexcept { return m_tempDirPath / "test.frag"; }
    [[nodiscard]] std::filesystem::path geomPath() const noexcept { return m_tempDirPath / "test.geom"; }
    [[nodiscard]] std::filesystem::path texPath() const noexcept { return m_tempDirPath / "test.png"; }
    [[nodiscard]] std::filesystem::path resourcePath() const noexcept { return m_tempDirPath; }

private:
    static constexpr auto SHADER_CODE{ R"(
        #version 330 core
        void main() {}
    )" };
    static constexpr auto IMAGE_WIDTH{ 1 };
    static constexpr auto IMAGE_HEIGHT{ 1 };
    static constexpr auto IMAGE_CHANNELS{ 4 };
    static constexpr std::array<unsigned char, 4> TEST_IMAGE_PIXELS{ 255, 255, 255, 255 };

    std::filesystem::path m_tempDirPath;

    void createTestShader()
    {
        auto path{ m_tempDirPath / "test.vert" };
        std::ofstream(path) << SHADER_CODE;

        path = m_tempDirPath / "test.frag";
        std::ofstream(path) << SHADER_CODE;

        path = m_tempDirPath / "test.geom";
        std::ofstream(path) << SHADER_CODE;
    }

    void createTestTexture()
    {
        auto path{ m_tempDirPath / "test.png" };

        stbi_write_png(
            path.c_str(),
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

