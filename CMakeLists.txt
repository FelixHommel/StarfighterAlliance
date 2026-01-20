cmake_minimum_required(VERSION 3.30)
project(
    StarfighterAlliance
    DESCRIPTION "Space invaders like game"
    LANGUAGES CXX
)

add_subdirectory(deps)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/vcpkg-dependencies.cmake)
include(cmake/DefaultProjectSettings.cmake)
include(cmake/CodeCoverage.cmake)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

enable_testing()
add_subdirectory(src)
