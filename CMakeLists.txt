cmake_minimum_required(VERSION 3.30)
project(
    StarfighterAlliance
    DESCRIPTION "Space invaders like game"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/DefaultProjectSettings.cmake)
include(cmake/CodeCoverage.cmake)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)

enable_testing()
add_subdirectory(deps)
add_subdirectory(src)
