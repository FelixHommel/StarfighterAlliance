#include "core/Texture.hpp"
#include "fixtures/OpenGLTestFixture.hpp"

#include <glad/gl.h>

#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <memory>
#include <utility>

namespace sfa::testing
{

/// \brief Test the features of the \ref Texture class.
///
/// \author Felix Hommel
/// \date 1/24/2026
class TextureTest : public ::testing::Test
{
public:
    TextureTest() = default;
    ~TextureTest() override = default;

    TextureTest(const TextureTest&) = delete;
    TextureTest(TextureTest&&) = delete;
    TextureTest& operator=(const TextureTest&) = delete;
    TextureTest& operator=(TextureTest&&) = delete;

    void SetUp() override
    {
        if(!m_context->setup())
            GTEST_SKIP() << m_context->getSkipReason();
    }

    void TearDown() override { m_context->teardown(); }

protected:
    static constexpr auto TEST_IMAGE_WIDTH{ 1 };
    static constexpr auto TEST_IMAGE_HEIGHT{ 1 };
    static constexpr auto TEST_IMAGE_CHANNELS{ 4 };
    static constexpr std::array TEST_IMAGE_PIXELS{ std::byte(255), std::byte(255), std::byte(255), std::byte(255) };

    std::unique_ptr<OpenGLTestFixture> m_context{ std::make_unique<OpenGLTestFixture>() };
};

/// \brief Test the \ref Texture RAII behavior.
///
/// The constructor should create a valid OpenGL texture and the destructor should delete the texture.
TEST_F(TextureTest, TextureRAII)
{
    unsigned int textureID{};
    {
        Texture2D texure(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, TEST_IMAGE_CHANNELS, TEST_IMAGE_PIXELS);
        textureID = texure.getID();

        EXPECT_NE(textureID, 0);
        EXPECT_TRUE(static_cast<bool>(glIsTexture(textureID)));
    }

    EXPECT_FALSE(static_cast<bool>(glIsTexture(textureID)));
}

/// \brief Test the \ref Texture move constructor.
///
/// Move constructing a \ref Texture should preserve the underlying texture ID.
TEST_F(TextureTest, TextureMoveConstructor)
{
    Texture2D original(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, TEST_IMAGE_CHANNELS, TEST_IMAGE_PIXELS);
    const unsigned int originalID{ original.getID() };
    Texture2D moved{ std::move(original) };

    EXPECT_EQ(moved.getID(), originalID);
    EXPECT_TRUE(static_cast<bool>(glIsTexture(moved.getID())));
}

/// \brief Test the \ref Texture move assignment.
///
/// Move assignment should preserve the move source's texture and clean up the texture the move target owned so far.
TEST_F(TextureTest, TextureMoveAssignment)
{
    Texture2D texture1(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, TEST_IMAGE_CHANNELS, TEST_IMAGE_PIXELS);
    const unsigned int originalTexture1ID{ texture1.getID() };
    Texture2D texture2(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, TEST_IMAGE_CHANNELS, TEST_IMAGE_PIXELS);
    const unsigned int originalTexture2ID{ texture2.getID() };

    texture2 = std::move(texture1);

    EXPECT_FALSE(static_cast<bool>(glIsTexture(originalTexture2ID)));
    EXPECT_EQ(originalTexture1ID, texture2.getID());
    EXPECT_TRUE(static_cast<bool>(glIsTexture(texture2.getID())));
}

/// \brief Test the \ref Texture bind method.
///
/// When a texture is bound it should be bound by the OpenGL state.
TEST_F(TextureTest, TextureBindActivatesTheTexture)
{
    Texture2D texture(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, TEST_IMAGE_CHANNELS, TEST_IMAGE_PIXELS);
    texture.bind();

    int boundTexture{ 0 };
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

    ASSERT_NE(0, boundTexture);
    EXPECT_EQ(boundTexture, texture.getID());
}

} // namespace sfa::testing

