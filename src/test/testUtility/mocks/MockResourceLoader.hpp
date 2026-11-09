#ifndef SFA_SRC_TEST_TEST_UTILITY_MOCKS_MOCK_RESOURCE_LOADER_HPP
#define SFA_SRC_TEST_TEST_UTILITY_MOCKS_MOCK_RESOURCE_LOADER_HPP

#include "core/resourceManagement/IResourceLoader.hpp"

#include <trompeloeil/mock.hpp>

namespace sfa::testing
{

/// \brief Mock implementation for \ref IResourceLoader to use in testing
///
/// \author Felix Hommel
/// \date 2/15/2026
class MockResourceLoader : public trompeloeil::mock_interface<IResourceLoader>
{
public:
    IMPLEMENT_MOCK3(loadShader);
    IMPLEMENT_MOCK1(loadTexture);
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_TEST_UTILITY_MOCKS_MOCK_RESOURCE_LOADER_HPP

