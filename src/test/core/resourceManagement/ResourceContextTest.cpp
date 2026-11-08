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
#include <thread>
#include <utility>

namespace
{

constexpr auto UPLOAD_DELAY{ std::chrono::milliseconds(1000) };
constexpr auto SHADER_NAME{ "shader" };
constexpr auto TEXTURE_NAME{ "texture" };

} // namespace

namespace sfa::testing
{

/// \brief Test the features of the \ref ResourceContext
///
/// \author Felix Hommel
/// \date 2/16/2026
TEST_CASE("ResourceContextTest", "[unit][core][opengl][concurrency]")
{
    const ShaderSourceData EXPECTED_SHADER_DATA{
        .vertexSource = "vertex shader source code",
        .fragmentSource = "fragment shader source code",
    };
    const TextureSourceData EXPECTED_TEXTURE_DATA{
        .width = 1,
        .height = 1,
        .channels = 4,
        .pixels = { std::byte(255), std::byte(255), std::byte(255), std::byte(255) },
    };

    ResourceGenerator generator{ Catch::getResultCapture().getCurrentTestName() };
    OpenGLTestFixture glContext{};

    if(!glContext.setup())
        SKIP(glContext.getSkipReason());

    /// \brief Request the loading of a shader resource.
    ///
    /// When a shader resource is requested, the processed shader source code is stored in the upload queue of the
    /// \ref ResourceContext, waiting to be processed by the main thread.
    SECTION("Load shader success")
    {
        auto mock{ std::make_unique<MockResourceLoader>() };
        MockResourceLoader* pMock{ mock.get() };
        ResourceContext context{ std::move(mock) };

        REQUIRE_CALL(*pMock, loadShader("", "", "")).RETURN(LoadResult{ EXPECTED_SHADER_DATA });

        context.requestResource(
            ResourceContext::ShaderLoadRequest{ .name = ::SHADER_NAME, .vert = "", .frag = "", .geom = "" }
        );
        std::this_thread::sleep_for(::UPLOAD_DELAY);

        CHECK(context.hasPendingUploads());
        CHECK(context.pendingUploadTasks() == 1);
    }

    SECTION("Load shader blocking success")
    {
        auto mock{ std::make_unique<MockResourceLoader>() };
        MockResourceLoader* pMock{ mock.get() };
        ResourceContext context{ std::move(mock) };

        REQUIRE_CALL(*pMock, loadShader("", "", "")).RETURN(LoadResult{ EXPECTED_SHADER_DATA });

        context.requestResourceBlocking(
            ResourceContext::ShaderLoadRequest{ .name = ::SHADER_NAME, .vert = "", .frag = "", .geom = "" }
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
        auto mock{ std::make_unique<MockResourceLoader>() };
        MockResourceLoader* pMock{ mock.get() };
        ResourceContext context{ std::move(mock) };

        REQUIRE_CALL(*pMock, loadTexture("")).RETURN(LoadResult{ EXPECTED_TEXTURE_DATA });

        context.requestResource(ResourceContext::TextureLoadRequest{ .name = ::TEXTURE_NAME, .filepath = "" });
        std::this_thread::sleep_for(::UPLOAD_DELAY);

        CHECK(context.hasPendingUploads());
        CHECK(context.pendingUploadTasks() == 1);
    }

    SECTION("Load texture blocking success")
    {
        auto mock{ std::make_unique<MockResourceLoader>() };
        MockResourceLoader* pMock{ mock.get() };
        ResourceContext context{ std::move(mock) };

        REQUIRE_CALL(*pMock, loadTexture("")).RETURN(LoadResult{ EXPECTED_TEXTURE_DATA });

        context.requestResourceBlocking(ResourceContext::TextureLoadRequest{ .name = ::TEXTURE_NAME, .filepath = "" });

        CHECK(context.totalResources() == 1);
        CHECK_NOTHROW(context.getTexture(::TEXTURE_NAME));
    }

    /// \brief Load multiple resources at the same time.
    ///
    /// When multiple resources are requested at the same time, the thread pool responsible for processing the initial
    /// request should handle them in parallel. The main thread should block until all resources are uploaded to the GPU.
    SECTION("Load multiple resources concurrently")
    {
        ResourceContext context{};

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

    /// \brief Clear the resource cache.
    ///
    /// When the resource cache is cleared, all currently stored resources are destroyed.
    SECTION("Clear resource context")
    {
        ResourceContext context{};

        context.requestResource(
            ResourceContext::ShaderLoadRequest{
                .name = ::SHADER_NAME, .vert = generator.vertPath(), .frag = generator.fragPath() }
        );
        context.requestResource(
            ResourceContext::TextureLoadRequest{ .name = ::TEXTURE_NAME, .filepath = generator.texPath() }
        );
        context.waitForAllUploads();

        REQUIRE(context.totalResources() == 2);

        context.clear();

        CHECK(context.totalResources() == 0);
    }

    glContext.teardown();
}

} // namespace sfa::testing

