#include "core/Texture.hpp"
#include "testUtility/OpenGLTestContext.hpp"

#include <catch2/catch_test_macros.hpp>
#include <glad/gl.h>

#include <array>
#include <cstddef>
#include <utility>

namespace
{

constexpr auto TEST_IMAGE_WIDTH{ 1 };
constexpr auto TEST_IMAGE_HEIGHT{ 1 };
constexpr auto TEST_IMAGE_CHANNELS{ 4 };
constexpr std::array TEST_IMAGE_PIXELS{ std::byte(255), std::byte(255), std::byte(255), std::byte(255) };

} // namespace

namespace sfa::testing
{

/// \brief Test the features of the \ref Texture class.
///
/// \author Felix Hommel
/// \date 1/24/2026
TEST_CASE("TextureTest", "[unit][core][opengl]")
{
    OpenGLTestFixture context{};

    if(!context.setup())
        SKIP(context.getSkipReason());

    /// \brief Test the \ref Texture RAII behavior.
    ///
    /// The constructor should create a valid OpenGL texture and the destructor should delete the texture.
    SECTION("Texture RAII")
    {
        unsigned int textureID{};
        {
            Texture2D texure(::TEST_IMAGE_WIDTH, ::TEST_IMAGE_HEIGHT, ::TEST_IMAGE_CHANNELS, ::TEST_IMAGE_PIXELS);
            textureID = texure.getID();

            CHECK(textureID != 0);
            CHECK(static_cast<bool>(glIsTexture(textureID)));
        }

        CHECK(!static_cast<bool>(glIsTexture(textureID)));
    }

    /// \brief Test the \ref Texture move constructor.
    ///
    /// Move constructing a \ref Texture should preserve the underlying texture ID.
    SECTION("Texture move constructor")
    {
        Texture2D original(::TEST_IMAGE_WIDTH, ::TEST_IMAGE_HEIGHT, ::TEST_IMAGE_CHANNELS, ::TEST_IMAGE_PIXELS);
        const unsigned int originalID{ original.getID() };
        Texture2D moved{ std::move(original) };

        CHECK(moved.getID() == originalID);
        CHECK(static_cast<bool>(glIsTexture(moved.getID())));
    }

    /// \brief Test the \ref Texture move assignment.
    ///
    /// Move assignment should preserve the move source's texture and clean up the texture the move target owned so far.
    SECTION("Texture move assignment")
    {
        Texture2D texture1(::TEST_IMAGE_WIDTH, ::TEST_IMAGE_HEIGHT, ::TEST_IMAGE_CHANNELS, ::TEST_IMAGE_PIXELS);
        const unsigned int originalTexture1ID{ texture1.getID() };
        Texture2D texture2(::TEST_IMAGE_WIDTH, ::TEST_IMAGE_HEIGHT, ::TEST_IMAGE_CHANNELS, ::TEST_IMAGE_PIXELS);
        const unsigned int originalTexture2ID{ texture2.getID() };

        texture2 = std::move(texture1);

        CHECK(!static_cast<bool>(glIsTexture(originalTexture2ID)));
        CHECK(texture2.getID() == originalTexture1ID);
        CHECK(static_cast<bool>(glIsTexture(texture2.getID())));
    }

    /// \brief Test the \ref Texture bind method.
    ///
    /// When a texture is bound it should be bound by the OpenGL state.
    SECTION("Texture bind activates the texture")
    {
        Texture2D texture(::TEST_IMAGE_WIDTH, ::TEST_IMAGE_HEIGHT, ::TEST_IMAGE_CHANNELS, ::TEST_IMAGE_PIXELS);
        texture.bind();

        int boundTexture{ 0 };
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

        CHECK(boundTexture != 0);
        CHECK(texture.getID() == boundTexture);
    }

    context.teardown();
}

} // namespace sfa::testing

