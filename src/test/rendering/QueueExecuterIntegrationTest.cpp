#include "rendering/RenderCommand.hpp"
#include "rendering/RenderExecutor.hpp"
#include "rendering/RenderQueue.hpp"
#include "testUtility/mocks/MockSpritePipeline.hpp"
#include "testUtility/mocks/MockTextPipeline.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <trompeloeil/mock.hpp>

#include <memory>
#include <utility>

namespace sfa::testing
{

TEST_CASE("QueueExecutorIntegrationTest", "[.][integration][rendering]")
{
    auto spritePipeline{ std::make_unique<MockSpritePipeline>() };
    const auto* const pSpritePipeline{ spritePipeline.get() };
    auto textPipeline{ std::make_unique<MockTextPipeline>() };
    const auto* const pTextPipeline{ textPipeline.get() };

    RenderExecutor executor{ std::move(spritePipeline), std::move(textPipeline) };
    RenderQueue queue{};

    queue.submit(SpriteSubmission{});
    queue.submit(TextSubmission{});
    queue.submit(SpriteSubmission{});

    CHECK(queue.size() == 3);

    REQUIRE_CALL(*pSpritePipeline, draw(trompeloeil::_)).TIMES(2);
    REQUIRE_CALL(*pTextPipeline, draw(trompeloeil::_)).TIMES(1);

    executor.execute(queue.commands());
}

} // namespace sfa::testing

