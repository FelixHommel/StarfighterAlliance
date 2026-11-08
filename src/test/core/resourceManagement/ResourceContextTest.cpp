#include "core/resourceManagement/ResourceContext.hpp"
#include "core/resourceManagement/IntermediateResourceData.hpp"
#include "mocks/MockResourceLoader.hpp"
#include "testUtility/OpenGLTestContext.hpp"
#include "testUtility/ResourceGenerator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/interfaces/catch_interfaces_capture.hpp>
#include <catch2/trompeloeil.hpp>
#include <trompeloeil/mock.hpp>

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <thread>
#include <utility>

namespace
{

constexpr auto COLOR_WHITE{ std::byte(255) };
constexpr auto UPLOAD_DELAY{ std::chrono::milliseconds(1000) };
constexpr auto SHADER_NAME{ "shader" };
constexpr auto TEXTURE_NAME{ "texture" };

} // namespace

namespace sfa::testing
{

/// \brief Test the \ref ResourceContext with a \ref MockResourceLoader.
///
/// \author Felix Hommel
/// \date 11/8/2026
TEST_CASE("ResourceContextMockTest", "[unit][core][opengl][concurrency]")
{
    const ShaderSourceData EXPECTED_SHADER_DATA{
        .vertexSource = "vertex shader source code",
        .fragmentSource = "fragment shader source code",
    };
    const TextureSourceData EXPECTED_TEXTURE_DATA{
        .width = 1,
        .height = 1,
        .channels = 4,
        .pixels = { ::COLOR_WHITE, ::COLOR_WHITE, ::COLOR_WHITE, ::COLOR_WHITE },
    };
    const auto MOCK_FS_PATH{ std::filesystem::path("") };

    OpenGLTestContext glContext{};
    if(!glContext.setup())
        SKIP(glContext.getSkipReason());

    auto mockLoader{ std::make_unique<MockResourceLoader>() };
    MockResourceLoader* const pMockLoader{ mockLoader.get() };
    ResourceContext context{ std::move(mockLoader) };

    /// \brief Request the loading of a shader resource.
    ///
    /// When a shader resource is requested, the processed shader source code is stored in the upload queue of the
    /// \ref ResourceContext, waiting to be processed by the main thread.
    SECTION("Load shader success")
    {
        REQUIRE_CALL(*pMockLoader, loadShader(MOCK_FS_PATH, MOCK_FS_PATH, MOCK_FS_PATH))
            .RETURN(LoadResult{ EXPECTED_SHADER_DATA })
            .TIMES(1);

        context.requestResource(
            ResourceContext::ShaderLoadRequest{
                .name = ::SHADER_NAME, .vert = MOCK_FS_PATH, .frag = MOCK_FS_PATH, .geom = MOCK_FS_PATH }
        );
        std::this_thread::sleep_for(::UPLOAD_DELAY);

        CHECK(context.hasPendingUploads());
        CHECK(context.pendingUploadTasks() == 1);
    }

    /// \brief Request the loading of a shader resource and block until finish.
    ///
    /// When a shader resource is requested, the processed shader source code is stored in the upload queue of the
    /// \ref ResourceContext, waiting to be processed by the main thread.
    SECTION("Load shader blocking success")
    {
        REQUIRE_CALL(*pMockLoader, loadShader(MOCK_FS_PATH, MOCK_FS_PATH, MOCK_FS_PATH))
            .RETURN(LoadResult{ EXPECTED_SHADER_DATA })
            .TIMES(1);

        context.requestResourceBlocking(
            ResourceContext::ShaderLoadRequest{
                .name = ::SHADER_NAME, .vert = MOCK_FS_PATH, .frag = MOCK_FS_PATH, .geom = MOCK_FS_PATH }
        );

        CHECK(context.totalResources() == 1);
        CHECK_NOTHROW(context.getShader(::SHADER_NAME));
    }

    /// \brief Request the loading of a texture resource.
    ///
    /// When a texture resource is requested, the processed texture data is stored in the upload queue of the
    /// \ref ResourceContext, waiting to be processed by the main thread.
    SECTION("Load texture success")
    {
        REQUIRE_CALL(*pMockLoader, loadTexture(MOCK_FS_PATH)).RETURN(LoadResult{ EXPECTED_TEXTURE_DATA }).TIMES(1);

        context.requestResource(
            ResourceContext::TextureLoadRequest{ .name = ::TEXTURE_NAME, .filepath = MOCK_FS_PATH }
        );
        std::this_thread::sleep_for(::UPLOAD_DELAY);

        CHECK(context.hasPendingUploads());
        CHECK(context.pendingUploadTasks() == 1);
    }

    /// \brief Request the loading of a texture resource and block until finish.
    ///
    /// When a texture resource is requested, the processed texture data is stored in the upload queue of the
    /// \ref ResourceContext, waiting to be processed by the main thread.
    SECTION("Load texture blocking success")
    {
        REQUIRE_CALL(*pMockLoader, loadTexture(MOCK_FS_PATH)).RETURN(LoadResult{ EXPECTED_TEXTURE_DATA }).TIMES(1);

        context.requestResourceBlocking(
            ResourceContext::TextureLoadRequest{ .name = ::TEXTURE_NAME, .filepath = MOCK_FS_PATH }
        );

        CHECK(context.totalResources() == 1);
        CHECK_NOTHROW(context.getTexture(::TEXTURE_NAME));
    }

    /// \brief Clear the resource cache.
    ///
    /// When the resource cache is cleared, all currently stored resources are destroyed.
    SECTION("Clear resource context")
    {
        ALLOW_CALL(*pMockLoader, loadShader(MOCK_FS_PATH, MOCK_FS_PATH, MOCK_FS_PATH))
            .RETURN(LoadResult{ EXPECTED_SHADER_DATA });
        ALLOW_CALL(*pMockLoader, loadTexture(MOCK_FS_PATH)).RETURN(LoadResult{ EXPECTED_TEXTURE_DATA });

        context.requestResourceBlocking(
            ResourceContext::ShaderLoadRequest{ .name = ::SHADER_NAME, .vert = MOCK_FS_PATH, .frag = MOCK_FS_PATH }
        );
        context.requestResourceBlocking(
            ResourceContext::TextureLoadRequest{ .name = ::TEXTURE_NAME, .filepath = MOCK_FS_PATH }
        );

        REQUIRE(context.totalResources() == 2);

        context.clear();

        CHECK(context.totalResources() == 0);
    }

    glContext.teardown();
}

/// \brief Test the features of the \ref ResourceContext with real filesystem interaction.
///
/// \author Felix Hommel
/// \date 2/16/2026
TEST_CASE("ResourceContextIntegrationTest", "[.][integration][core][opengl][concurrency][filesystem]")
{
    const ResourceGenerator generator{ Catch::getResultCapture().getCurrentTestName() };

    OpenGLTestContext glContext{};
    if(!glContext.setup())
        SKIP(glContext.getSkipReason());

    ResourceContext context{};

    /// \brief Load multiple resources at the same time.
    ///
    /// When multiple resources are requested at the same time, the thread pool responsible for processing the initial
    /// request should handle them in parallel. The main thread should block until all resources are uploaded to the GPU.
    SECTION("Load multiple resources concurrently")
    {
        context.requestResource(
            ResourceContext::ShaderLoadRequest{
                .name = ::SHADER_NAME, .vert = generator.vertPath(), .frag = generator.fragPath() }
        );
        context.requestResource(
            ResourceContext::TextureLoadRequest{ .name = ::TEXTURE_NAME, .filepath = generator.texPath() }
        );
        context.waitForAllUploads();

        CHECK(context.totalResources() == 2);
        CHECK(!context.hasPendingUploads());
    }

    glContext.teardown();
}

} // namespace sfa::testing

