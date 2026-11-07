#include "core/resourceManagement/ResourceLoader.hpp"

#include "testUtility/ResourceGenerator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/interfaces/catch_interfaces_capture.hpp>

namespace sfa::testing
{

/// \brief Test the features of the \ref ResourceLoader.
///
/// \author Felix Hommel
/// \date 2/16/2026
TEST_CASE("ResourceLoaderTest", "[unit][core]")
{
    ResourceGenerator generator{ Catch::getResultCapture().getCurrentTestName() };
    ResourceLoader loader{};

    /// \brief Test loading shader files from disk.
    ///
    /// If all shader files are present the loader should be successfully reading them and storing them in their dedicated
    /// storage structure, resulting in an expected value.
    SECTION("Load shader expected")
    {
        const auto r{ loader.loadShader(generator.vertPath(), generator.fragPath(), generator.geomPath()) };

        CHECK(r.has_value());
    }

    /// \brief Try loading a shader with an invalid vertex shader path.
    ///
    /// When the vertex shader path is invalid, it should result in a std::unexpected return value.
    SECTION("Load shader unexpected vertex shader file")
    {
        const auto r{ loader.loadShader("non-existing-vert", generator.fragPath()) };

        CHECK(!r.has_value());
    }

    /// \brief Try loading a shader with an invalid fragment shader path.
    ///
    /// When the fragment shader path is invalid, it should result in a std::unexpected return value.
    SECTION("Load shader unexpected fragment Shader file")
    {
        const auto r{ loader.loadShader(generator.vertPath(), "non-existing-frag") };

        CHECK(!r.has_value());
    }

    /// \brief Try loading a shader with an invalid geometry shader path.
    ///
    /// When the geometry shader path is invalid, it should result in a std::unexpected return value.
    SECTION("Load shader unexpected geometry shader file")
    {
        const auto r{ loader.loadShader(generator.vertPath(), generator.fragPath(), "non-existing-geom") };

        CHECK(!r.has_value());
    }

    /// \brief Load a valid texture resource from the disk.
    ///
    /// When a path to a valid texture resource is supplied, the method should return a std::expected object containing
    /// the texture's data.
    SECTION("Load texture expected")
    {
        const auto r{ loader.loadTexture(generator.texPath()) };

        CHECK(r.has_value());
    }

    /// \brief Try loading a texture resource from an invalid source on the disk.
    ///
    /// When the provided path is leading to an invalid resource, it should result in a std::unexpected return value.
    SECTION("Load texture unexpected")
    {
        const auto r{ loader.loadTexture(".") };

        CHECK(!r.has_value());
    }

    /// \brief Try loading a texture resource from a non-existing file.
    ///
    /// When the provided path is not leading to an existing file, it should result in a std::unexpected return value.
    SECTION("Load texture from non existing file")
    {
        const auto r{ loader.loadTexture("non-existing-texture") };

        CHECK(!r.has_value());
    }
}

} // namespace sfa::testing

