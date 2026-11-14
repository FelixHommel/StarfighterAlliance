#include "rendering/RenderQueue.hpp"
#include "rendering/RenderCommand.hpp"

#include <catch2/catch_test_macros.hpp>

namespace sfa::testing
{

TEST_CASE("RenderQueueTest", "[unit][rendering]")
{
    RenderQueue queue{};

    SECTION("newly constructed queue is empty")
    {
        CHECK(queue.empty());
    }

    SECTION("submitted commands are registered")
    {
        queue.submit(SpriteSubmission{});
        queue.submit(TextSubmission{});

        CHECK(queue.size() == 2);
    }

    SECTION("submitted commands can be retrieved")
    {
        queue.submit(SpriteSubmission{});
        queue.submit(TextSubmission{});

        const auto commands{ queue.commands() };

        CHECK(commands.size() == 2);
    }
}

} // namespace sfa::testing

