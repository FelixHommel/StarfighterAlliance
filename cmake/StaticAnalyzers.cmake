if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    option(SFA_ENABLE_CLANG_TIDY "Enable clang-tidy during compilation" ON)

    if(SFA_ENABLE_CLANG_TIDY)
        find_program(CLANGTIDY clang-tidy)

        if(CLANGTIDY)
            message(STATUS "Using clang-tidy")
            set(CMAKE_CXX_CLANG_TIDY
                "${CLANGTIDY};-extra-arg=-Wno-unknown-warning-option"
            )

            if(WIN32)
                set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY};--extra-arg=/EHsc")
            endif()
        else()
            message(WARNING "clang-tidy requested but executable not found")
        endif()
    else()
        message(STATUS "clang-tidy integration disabled")
    endif()

    option(
        SFA_ENABLE_ADDRESS_SANITIZER
        "Prepare the build to compile with address sanitizer"
        ON
    )
    option(SFA_ENABLE_VALGRIND "Prepare the build to be used with valgrind" OFF)
    if(NOT (SFA_ENABLE_ADDRESS_SANITIZER AND SFA_ENABLE_VALGRIND))
        if((NOT WIN32) AND SFA_ENABLE_ADDRESS_SANITIZER)
            message(STATUS "configuring asan build")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address")
        elseif((NOT WIN32) AND SFA_ENABLE_VALGRIND)
            message(STATUS "configuring valgrind build")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
        endif()
    else()
        message(
            WARNING
            "Can not use asan and valgrind at the same time, neither was enabled."
        )
    endif()
endif()
