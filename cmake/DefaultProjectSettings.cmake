if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting buidl type to 'Debug' because none other was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

find_program(CCACHE ccache)
if(CCACHE)
    message(STATUS "Using ccache")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
    message(STATUS "Ccache not found")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SFA_DEBUG "Enable debug statements and asserts" OFF)
option(SFA_ENABLE_ASSERTIONS "Enable assertions" OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SFA_DEBUG ON CACHE BOOL "" FORCE)
    set(SFA_ENABLE_ASSERTIONS ON CACHE BOOL "" FORCE)
endif()

add_compile_definitions(SFA_DEBUG=$<BOOL:${SFA_DEBUG}>)
add_compile_definitions(SFA_ENABLE_ASSERTIONS=$<BOOL:${SFA_ENABLE_ASSERTIONS}>)

