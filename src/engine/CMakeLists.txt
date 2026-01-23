set(NAME "StarfighterAllianceEngine")
include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME}
    ./utility/Button.cpp
    ./utility/ParticleGenerator.cpp
    ./utility/ResourceManager.cpp
    ./utility/Shader.cpp
    ./utility/SpriteRenderer.cpp
    ./utility/TextRenderer.cpp
    ./utility/Texture.cpp
    ./utility/resourceManagement/ResourceContext.cpp
    ./utility/ui/Layout.cpp
    ./utility/ui/VerticalLayout.cpp
    ./external/stb_image_impl.cpp
)

target_sources(${NAME}
    PRIVATE
        FILE_SET HEADERS
        FILES
            ./utility/Button.hpp
            ./utility/JSONReader.hpp
            ./utility/ParticleGenerator.hpp
            ./utility/ResourceManager.hpp
            ./utility/Shader.hpp
            ./utility/SpriteRenderer.hpp
            ./utility/TextRenderer.hpp
            ./utility/Texture.hpp
            ./utility/resourceManagement/ResourceCache.hpp
            ./utility/resourceManagement/ResourceContext.hpp
            ./utility/ui/Anchor.hpp
            ./utility/ui/Layout.hpp
            ./utility/ui/LayoutElement.hpp
            ./utility/ui/Size.hpp
            ./utility/ui/VerticalLayout.hpp
            ./external/stb_image.h
)

target_compile_features(${NAME} PRIVATE cxx_std_23)
target_link_libraries(${NAME}
    PRIVATE
        project_warnings
        OpenGL::GL
        Glad2CMake::glad2cmake
        SQLiteCpp
        Freetype::Freetype
        nlohmann_json::nlohmann_json
        glm::glm
        glfw
        spdlog::spdlog
        valijson
        fmt::fmt
)
target_include_directories(${NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
