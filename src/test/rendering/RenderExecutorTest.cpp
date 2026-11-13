#include "rendering/RenderExecutor.hpp"
#include "rendering/RenderCommand.hpp"
#include "testUtility/mocks/MockSpritePipeline.hpp"
#include "testUtility/mocks/MockTextPipeline.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <trompeloeil/mock.hpp>

#include <array>
#include <memory>
#include <utility>

namespace sfa::testing
{

TEST_CASE("RenderExecutorTest", "[unit][rendering]")
{
    auto spritePipeline{ std::make_unique<MockSpritePipeline>() };
    const auto* const pSpritePipeline{ spritePipeline.get() };
    auto textPipeline{ std::make_unique<MockTextPipeline>() };
    const auto* const pTextPipeline{ textPipeline.get() };

    RenderExecutor executor{ std::move(spritePipeline), std::move(textPipeline) };

    SECTION("submission of sprite renderable calls draw in the ISpritePipeline implementation")
    {
        const std::array<RenderCommand, 1> commands{ SpriteSubmission{} };

        REQUIRE_CALL(*pSpritePipeline, draw(trompeloeil::_)).TIMES(1);

        executor.execute(commands);
    }

    SECTION("submission of text renderable calls draw in the ITextPipeline implementation")
    {
        const std::array<RenderCommand, 1> commands{ TextSubmission{} };

        REQUIRE_CALL(*pTextPipeline, draw(trompeloeil::_)).TIMES(1);

        executor.execute(commands);
    }

    SECTION(
        "submission of sprite and text renderable calls draw in the ISpritePipeline and ITextPipeline implementation"
    )
    {
        const std::array<RenderCommand, 2> commands{ SpriteSubmission{}, TextSubmission{} };

        REQUIRE_CALL(*pTextPipeline, draw(trompeloeil::_)).TIMES(1);
        REQUIRE_CALL(*pSpritePipeline, draw(trompeloeil::_)).TIMES(1);

        executor.execute(commands);
    }
}

} // namespace sfa::testing

