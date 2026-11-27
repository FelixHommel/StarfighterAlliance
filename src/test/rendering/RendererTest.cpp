#include "rendering/Renderer.hpp"
#include "rendering/RenderCommand.hpp"
#include "testUtility/mocks/MockSpritePipeline.hpp"
#include "testUtility/mocks/MockTextPipeline.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/trompeloeil.hpp>
#include <trompeloeil/mock.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

namespace sfa::testing
{

TEST_CASE("RendererTest", "[unit][rendering]")
{
    using Catch::Matchers::ContainsSubstring;
    using Catch::Matchers::MessageMatches;

    auto spritePipeline{ std::make_unique<MockSpritePipeline>() };
    auto* const pSpritePipeline{ spritePipeline.get() };
    auto textPipeline{ std::make_unique<MockTextPipeline>() };
    auto* const pTextPipeline{ textPipeline.get() };

    Renderer renderer{ std::move(spritePipeline), std::move(textPipeline) };

    ALLOW_CALL(*pSpritePipeline, beginFrame(trompeloeil::_));
    ALLOW_CALL(*pSpritePipeline, draw(trompeloeil::_));
    ALLOW_CALL(*pSpritePipeline, endFrame());
    ALLOW_CALL(*pTextPipeline, beginFrame(trompeloeil::_));
    ALLOW_CALL(*pTextPipeline, draw(trompeloeil::_));
    ALLOW_CALL(*pTextPipeline, endFrame());

    SECTION("Cannot begin two frames")
    {
        CHECK_NOTHROW(renderer.beginFrame({}));
        CHECK_THROWS_MATCHES(
            renderer.beginFrame({}), std::runtime_error, MessageMatches(ContainsSubstring("Cannot start a new frame"))
        );
    }

    SECTION("Cannot end a frame before it was started")
    {
        CHECK_THROWS_MATCHES(
            renderer.endFrame(),
            std::runtime_error,
            MessageMatches(ContainsSubstring("no frame is currently being rendered"))
        );
    }

    SECTION("Submiting when no frame is active does not fataly fail")
    {
        CHECK_NOTHROW(renderer.submit(SpriteSubmission{}));
    }

    SECTION("Renderer beginFrame calls beginFrame of the pipelines")
    {
        REQUIRE_CALL(*pSpritePipeline, beginFrame(trompeloeil::_)).TIMES(1);
        REQUIRE_CALL(*pTextPipeline, beginFrame(trompeloeil::_)).TIMES(1);

        renderer.beginFrame({});
    }

    SECTION("Renderer endFrame calls endFrame of the pipelines")
    {
        renderer.beginFrame({});

        REQUIRE_CALL(*pSpritePipeline, endFrame()).TIMES(1);
        REQUIRE_CALL(*pTextPipeline, endFrame()).TIMES(1);

        renderer.endFrame();
    }
}

} // namespace sfa::testing

