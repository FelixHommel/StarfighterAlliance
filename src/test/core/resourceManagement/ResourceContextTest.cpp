#include "core/resourceManagement/ResourceContext.hpp"

#include "core/resourceManagement/IntermediateResourceData.hpp"
#include "fixtures/OpenGLTestFixture.hpp"
#include "mocks/MockResourceLoader.hpp"
#include "testUtility/ResourceGenerator.hpp"

#include "gmock/gmock.h"
#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <thread>
#include <utility>

namespace sfa::testing
{

/// \brief Test the features of the \ref ResourceContext
///
/// \author Felix Hommel
/// \date 2/16/2026
class ResourceContextTest : public ::testing::Test
{
public:
    ResourceContextTest() = default;
    ~ResourceContextTest() override = default;

    ResourceContextTest(const ResourceContextTest&) = delete;
    ResourceContextTest& operator=(const ResourceContextTest&) = delete;
    ResourceContextTest(ResourceContextTest&&) noexcept = delete;
    ResourceContextTest& operator=(ResourceContextTest&&) noexcept = delete;

protected:
    static constexpr auto UPLOAD_DELAY{ std::chrono::milliseconds(1000) };

    std::unique_ptr<ResourceGenerator> m_generator{
        std::make_unique<ResourceGenerator>(::testing::UnitTest::GetInstance()->current_test_info())
    };

    void SetUp() override
    {
        if(!m_glContext->setup())
            GTEST_SKIP() << m_glContext->getSkipReason();
    }

    void TearDown() override { m_glContext->teardown(); }

private:
    std::unique_ptr<OpenGLTestFixture> m_glContext{ std::make_unique<OpenGLTestFixture>() };
};

/// \brief Request the loading of a shader resource.
///
/// When a shader resource is requested, the processed shader source code is stored in the upload queue of the
/// \ref ResourceContext, waiting to be processed by the main thread.
TEST_F(ResourceContextTest, LoadShaderSuccess)
{
    auto mock{ std::make_unique<MockResourceLoader>() };
    MockResourceLoader* pMock{ mock.get() };
    ResourceContext context{ std::move(mock) };

    const ShaderSourceData expectedData{ .vertexSource = "vertex shader source code",
                                         .fragmentSource = "fragment shader source code" };

    const std::filesystem::path p("");
    EXPECT_CALL(*pMock, loadShader(p, p, p)).WillOnce(::testing::Return(LoadResult{ expectedData }));

    context.requestResource(ResourceContext::ShaderLoadRequest{ .name = "shader", .vert = "", .frag = "", .geom = "" });
    std::this_thread::sleep_for(UPLOAD_DELAY);

    EXPECT_TRUE(context.hasPendingUploads());
    EXPECT_EQ(1, context.pendingUploadTasks());
}

/// \brief Request the loading of a texture resource.
///
/// When a texture resource is requested, the processed texture data is stored in the upload queue of the
/// \ref ResourceContext, waiting to be processed by the main thread.
TEST_F(ResourceContextTest, LoadTextureSuccess)
{
    auto mock{ std::make_unique<MockResourceLoader>() };
    MockResourceLoader* pMock{ mock.get() };
    ResourceContext context{ std::move(mock) };

    const TextureRawData expectedData{
        .width = 1,
        .height = 1,
        .channels = 4,
        .pixels = { std::byte(255), std::byte(255), std::byte(255), std::byte(255) }
    };

    const std::filesystem::path p("");
    EXPECT_CALL(*pMock, loadTexture(p)).WillOnce(::testing::Return(LoadResult{ expectedData }));

    context.requestResource(ResourceContext::TextureLoadRequest{ .name = "texture", .filepath = "" });
    std::this_thread::sleep_for(UPLOAD_DELAY);

    EXPECT_TRUE(context.hasPendingUploads());
    EXPECT_EQ(1, context.pendingUploadTasks());
}

/// \brief Load multiple resources at the same time.
///
/// When multiple resources are requested at the same time, the thread pool responsible for processing the initial
/// request should handle them in parallel. The main thread should block until all resources are uploaded to the GPU.
TEST_F(ResourceContextTest, LoadMultipleResourcesConcurrently)
{
    ResourceContext context{};

    context.requestResource(
        ResourceContext::ShaderLoadRequest{
            .name = "shader", .vert = m_generator->vertPath(), .frag = m_generator->fragPath() }
    );
    context.requestResource(
        ResourceContext::TextureLoadRequest{ .name = "texture", .filepath = m_generator->texPath() }
    );
    context.waitForAllUploads();

    EXPECT_EQ(2, context.totalResources());
    EXPECT_FALSE(context.hasPendingUploads());
}

/// \brief Clear the resource cache.
///
/// When the resource cache is cleared, all currently stored resources are destroyed.
TEST_F(ResourceContextTest, ClearResourceContext)
{
    ResourceContext context{};

    context.requestResource(
        ResourceContext::ShaderLoadRequest{
            .name = "shader", .vert = m_generator->vertPath(), .frag = m_generator->fragPath() }
    );
    context.requestResource(
        ResourceContext::TextureLoadRequest{ .name = "texture", .filepath = m_generator->texPath() }
    );
    context.waitForAllUploads();

    ASSERT_EQ(2, context.totalResources());

    context.clear();

    EXPECT_EQ(0, context.totalResources());
}

} // namespace sfa::testing

