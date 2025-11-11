include(FetchContent)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY "https://github.com/freetype/freetype.git"
    GIT_TAG VER-2-14-1
)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY "https://github.com/zaphoyd/websocketpp.git"
    GIT_TAG 0.8.2
)
FetchContent_MakeAvailable(websocketpp)

FetchContent_Declare(
  glad2
  GIT_REPOSITORY "https://github.com/FelixHommel/Glad2.git"
  GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(glad2)

FetchContent_Declare(
  sqlitecpp
  GIT_REPOSITORY "https://github.com/SRombauts/SQLiteCpp.git"
  GIT_TAG 3.3.2
)
FetchContent_MakeAvailable(sqlitecpp)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
add_subdirectory(googletest)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
add_subdirectory(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
add_subdirectory(glm)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.2
)
add_subdirectory(spdlog)

FetchContent_Declare(
    nlohmannjson
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
add_subdirectory(nlohmannjson)

FetchContent_Declare(
    valijson
    GIT_REPOSITORY https://github.com/tristanpenman/valijson.git
    GIT_TAG v1.0.6
)
add_subdirectory(valijson)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 11.2.0
)
add_subdirectory(fmt)
