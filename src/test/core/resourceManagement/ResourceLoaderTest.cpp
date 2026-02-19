#include "core/resourceManagement/ResourceLoader.hpp"

#include "testUtility/ResourceGenerator.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <memory>

namespace sfa::testing
{

/// \brief Test the features of the \ref ResourceLoader.
///
/// \author Felix Hommel
/// \date 2/16/2026
class ResourceLoaderTest : public ::testing::Test
{
public:
    ResourceLoaderTest() = default;
    ~ResourceLoaderTest() override = default;

    ResourceLoaderTest(const ResourceLoaderTest&) = delete;
    ResourceLoaderTest& operator=(const ResourceLoaderTest&) = delete;
    ResourceLoaderTest(ResourceLoaderTest&&) noexcept = delete;
    ResourceLoaderTest& operator=(ResourceLoaderTest&&) noexcept = delete;

protected:
    std::unique_ptr<ResourceGenerator> m_generator{
        std::make_unique<ResourceGenerator>(::testing::UnitTest::GetInstance()->current_test_info())
    };
    std::unique_ptr<ResourceLoader> m_loader{ std::make_unique<ResourceLoader>() };
};

/// \brief Test loading shader files from disk.
///
/// If all shader files are present the loader should be successfully reading them and storing them in their dedicated
/// storage structure, resulting in an expected value.
TEST_F(ResourceLoaderTest, LoadShaderExpected)
{
    const auto r{ m_loader->loadShader(m_generator->vertPath(), m_generator->fragPath(), m_generator->geomPath()) };

    EXPECT_TRUE(r.has_value());
}

/// \brief Try loading a shader with an invalid vertex shader path.
///
/// When the vertex shader path is invalid, it should result in a std::unexpected return value.
TEST_F(ResourceLoaderTest, LoadShaderUnexpectedVertexShaderFile)
{
    const auto r{ m_loader->loadShader("non-existing-vert", m_generator->fragPath()) };

    EXPECT_FALSE(r.has_value());
}

/// \brief Try loading a shader with an invalid fragment shader path.
///
/// When the fragment shader path is invalid, it should result in a std::unexpected return value.
TEST_F(ResourceLoaderTest, LoadShaderUnexpectedFragmentShaderFile)
{
    const auto r{ m_loader->loadShader(m_generator->vertPath(), "non-existing-frag") };

    EXPECT_FALSE(r.has_value());
}

/// \brief Try loading a shader with an invalid geometry shader path.
///
/// When the geometry shader path is invalid, it should result in a std::unexpected return value.
TEST_F(ResourceLoaderTest, LoadShaderUnexpectedGeometryShaderFile)
{
    const auto r{ m_loader->loadShader(m_generator->vertPath(), m_generator->fragPath(), "non-existing-geom") };

    EXPECT_FALSE(r.has_value());
}

/// \brief Load a valid texture resource from the disk.
///
/// When a path to a valid texture resource is supplied, the method should return a std::expected object containing
/// the texture's data.
TEST_F(ResourceLoaderTest, LoadTextureExpected)
{
    const auto r{ m_loader->loadTexture(m_generator->texPath()) };

    EXPECT_TRUE(r.has_value());
}

/// \brief Try loading a texture resource from an invalid source on the disk.
///
/// When the provided path is leading to an invalid resource, it should result in a std::unexpected return value.
TEST_F(ResourceLoaderTest, LoadTextureUnexpected)
{
    const auto r{ m_loader->loadTexture(".") };

    EXPECT_FALSE(r.has_value());
}

/// \brief Try loading a texture resource from a non-existing file.
///
/// When the provided path is not leading to an existing file, it should result in a std::unexpected return value.
TEST_F(ResourceLoaderTest, LoadTextureFromNonExistingFile)
{
    const auto r{ m_loader->loadTexture("non-existing-texture") };

    EXPECT_FALSE(r.has_value());
}

} // namespace sfa::testing

