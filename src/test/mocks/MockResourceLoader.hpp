#ifndef SFA_SRC_TEST_MOCKS_MOCK_RESOURCE_LOADER_HPP
#define SFA_SRC_TEST_MOCKS_MOCK_RESOURCE_LOADER_HPP

#include "core/resourceManagement/IResourceLoader.hpp"
#include "core/resourceManagement/IntermediateResourceData.hpp"

#include "gmock/gmock.h"

#include <filesystem>

namespace sfa::testing
{

/// \brief Mock implementation for \ref IResourceLoader to use in testing
///
/// \author Felix Hommel
/// \date 2/15/2026
class MockResourceLoader : public IResourceLoader
{
public:
    MOCK_METHOD(
        LoadResult,
        loadShader,
        (const std::filesystem::path&, const std::filesystem::path&, const std::filesystem::path&),
        (override)
    );
    MOCK_METHOD(LoadResult, loadTexture, (const std::filesystem::path&), (override));
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_MOCKS_MOCK_RESOURCE_LOADER_HPP

