cmake_minimum_required(VERSION 3.30)
project(
    StarfighterAlliance
    DESCRIPTION "Space invaders like game"
    LANGUAGES CXX
    VERSION 2.0.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_definitions(SFA_ROOT="${CMAKE_SOURCE_DIR}/")

include(cmake/vcpkg-dependencies.cmake)
include(cmake/DefaultProjectSettings.cmake)
include(cmake/CodeCoverage.cmake)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

enable_testing()
add_subdirectory(src)
