set(NAME "StarfighterAllianceTests")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(${NAME}
    ./main.cpp
    ./my_test.cpp
)

target_sources(${NAME}
    PRIVATE
        FILE_SET HEADERS
        FILES
            ./MockTexture.hpp
)

target_compile_features(${NAME} PRIVATE cxx_std_23)
target_link_libraries(${NAME}
    PRIVATE
        project_warnings
        GTest::gtest
        GTest::gmock
        StarfighterAllianceEngine
)

add_test(NAME ${NAME} COMMAND ${NAME})
