#include "rendering/RenderExecutor.hpp"
#include "rendering/RenderCommand.hpp"
#include "testUtility/mocks/MockSpritePipeline.hpp"
#include "testUtility/mocks/MockTextPipeline.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <trompeloeil/mock.hpp>

#include <array>

namespace sfa::testing
{

TEST_CASE("RenderExecutorTest", "[unit][rendering]")
{
    MockSpritePipeline spritePipeline{};
    MockTextPipeline textPipeline{};

    ALLOW_CALL(spritePipeline, endFrame());
    ALLOW_CALL(textPipeline, endFrame());

    RenderExecutor executor{ spritePipeline, textPipeline };

    SECTION("submission of sprite renderable calls draw in the ISpritePipeline implementation")
    {
        const std::array<RenderCommand, 1> commands{ SpriteSubmission{} };

        REQUIRE_CALL(spritePipeline, draw(trompeloeil::_)).TIMES(1);

        executor.execute(commands);
    }

    SECTION("submission of text renderable calls draw in the ITextPipeline implementation")
    {
        const std::array<RenderCommand, 1> commands{ TextSubmission{} };

        REQUIRE_CALL(textPipeline, draw(trompeloeil::_)).TIMES(1);

        executor.execute(commands);
    }

    SECTION(
        "submission of sprite and text renderable calls draw in the ISpritePipeline and ITextPipeline implementation"
    )
    {
        const std::array<RenderCommand, 2> commands{ SpriteSubmission{}, TextSubmission{} };

        REQUIRE_CALL(textPipeline, draw(trompeloeil::_)).TIMES(1);
        REQUIRE_CALL(spritePipeline, draw(trompeloeil::_)).TIMES(1);

        executor.execute(commands);
    }
}

} // namespace sfa::testing

