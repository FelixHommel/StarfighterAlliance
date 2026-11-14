set(NAME "StarfighterAllianceTests")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(
    ${NAME}
    ./concurrency/AsyncValueTest.cpp
    ./concurrency/BlockingQueueTest.cpp
    ./concurrency/ThreadPoolTest.cpp
    ./core/ShaderTest.cpp
    ./core/TextureTest.cpp
    ./core/resourceManagement/ResourceCacheTest.cpp
    ./core/resourceManagement/ResourceContextTest.cpp
    ./core/resourceManagement/ResourceLoaderTest.cpp
    ./ecs/ComponentArrayTest.cpp
    ./ecs/EntityManagerTest.cpp
    ./ecs/ComponentRegistryTest.cpp
    ./testUtility/OpenGLTestContext.cpp
    ./testUtility/stb_image_write_impl.cpp
    ./testUtility/TemporaryDirectory.cpp
    ./rendering/RenderQueueTest.cpp
    ./screens/ScreenStackTest.cpp
    ./utility/exceptions/ExceptionTest.cpp
    ./utility/exceptions/ResourceUnavailableExceptionTest.cpp
    ./utility/exceptions/WindowCreationExceptionTest.cpp
    ./utility/json/JsonReaderTest.cpp
    ./utility/userInput/InputControllerTest.cpp
)

target_sources(
    ${NAME}
    PRIVATE
        FILE_SET HEADERS
            FILES
                ./testUtility/OpenGLTestContext.hpp
                ./testUtility/RandomNumberGenerator.hpp
                ./testUtility/ResourceGenerator.hpp
                ./testUtility/TemporaryDirectory.hpp
                ./testUtility/TemporaryJsonFile.hpp
                ./testUtility/mocks/MockOverlayScreen.hpp
                ./testUtility/mocks/MockResourceLoader.hpp
                ./testUtility/mocks/MockScreen.hpp
                ./testUtility/mocks/MockWindow.hpp
)

target_compile_features(${NAME} PRIVATE cxx_std_23)
target_compile_options(
    ${NAME}
    INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS}>
)
target_link_libraries(
    ${NAME}
    PRIVATE
        Catch2::Catch2WithMain
        trompeloeil::trompeloeil
        StarfighterAllianceEngine
)

include(Catch)
include(CTest)
catch_discover_tests(${NAME})
