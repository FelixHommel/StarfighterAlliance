#ifndef SFA_SRC_TEST_TEST_UTILITY_MOCKS_MOCK_SPRITE_PIPELINE_HPP
#define SFA_SRC_TEST_TEST_UTILITY_MOCKS_MOCK_SPRITE_PIPELINE_HPP

#include "rendering/pipelines/ISpritePipeline.hpp"

#include <trompeloeil/mock.hpp>

namespace sfa::testing
{

/// \brief Mock implementation of the \ref ISpritePipeline interface.
///
/// \author Felix Hommel
/// \date 11/13/2026
class MockSpritePipeline : public trompeloeil::mock_interface<ISpritePipeline>
{
    IMPLEMENT_MOCK1(beginFrame);
    IMPLEMENT_MOCK0(endFrame);

    IMPLEMENT_MOCK1(draw);
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_TEST_UTILITY_MOCKS_MOCK_SPRITE_PIPELINE_HPP

