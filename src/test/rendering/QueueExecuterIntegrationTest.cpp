#include "rendering/RenderCommand.hpp"
#include "rendering/RenderExecutor.hpp"
#include "rendering/RenderQueue.hpp"
#include "testUtility/mocks/MockSpritePipeline.hpp"
#include "testUtility/mocks/MockTextPipeline.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <trompeloeil/mock.hpp>

namespace sfa::testing
{

TEST_CASE("QueueExecutorIntegrationTest", "[.][integration][rendering]")
{
    MockSpritePipeline spritePipeline{};
    MockTextPipeline textPipeline{};

    ALLOW_CALL(spritePipeline, endFrame());
    ALLOW_CALL(textPipeline, endFrame());

    RenderExecutor executor{ spritePipeline, textPipeline };
    RenderQueue queue{};

    queue.submit(SpriteSubmission{});
    queue.submit(TextSubmission{});
    queue.submit(SpriteSubmission{});

    CHECK(queue.size() == 3);

    REQUIRE_CALL(spritePipeline, draw(trompeloeil::_)).TIMES(2);
    REQUIRE_CALL(textPipeline, draw(trompeloeil::_)).TIMES(1);

    executor.execute(queue.commands());
}

} // namespace sfa::testing

