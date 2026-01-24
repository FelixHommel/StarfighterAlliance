set(NAME "StarfighterAlliance")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(${NAME}
    ./main.cpp
)

target_compile_features(${NAME} PRIVATE cxx_std_26)
target_link_libraries(${NAME}
    PRIVATE
        project_warnings
        StarfighterAllianceEngine
)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
