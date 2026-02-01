set(NAME "StarfighterAllianceEngine")
include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME}
    ./core/ParticleGenerator.cpp
    ./core/Shader.cpp
    ./core/SpriteRenderer.cpp
    ./core/TextRenderer.cpp
    ./core/Texture.cpp
    ./core/resourceManagement/ResourceContext.cpp
    ./ecs/EntityManager.cpp
    ./ecs/systems/ButtonSystem.cpp
    ./ecs/systems/LayoutSystem.cpp
    ./ecs/systems/MovementSystem.cpp
    ./ecs/systems/SpriteRenderSystem.cpp
    ./ecs/systems/TextRenderSystem.cpp
    ./external/stb_image_impl.cpp
)

target_sources(${NAME}
    PRIVATE
        FILE_SET HEADERS
        FILES
            ./core/ParticleGenerator.hpp
            ./core/Shader.hpp
            ./core/SpriteRenderer.hpp
            ./core/TextRenderer.hpp
            ./core/Texture.hpp
            ./core/Utility.hpp
            ./core/resourceManagement/ResourceCache.hpp
            ./core/resourceManagement/ResourceContext.hpp
            ./ecs/ComponentArray.hpp
            ./ecs/ComponentRegistry.hpp
            ./ecs/ECSUtility.hpp
            ./ecs/EntityManager.hpp
            ./ecs/IComponentArray.hpp
            ./ecs/components/AnchorComponent.hpp
            ./ecs/components/BoxColliderComponent.hpp
            ./ecs/components/ButtonComponent.hpp
            ./ecs/components/CircleColliderComponent.hpp
            ./ecs/components/DamageComponent.hpp
            ./ecs/components/HealthComponent.hpp
            ./ecs/components/IComponent.hpp
            ./ecs/components/LayoutComponent.hpp
            ./ecs/components/RigidBodyComponent.hpp
            ./ecs/components/SpriteComponent.hpp
            ./ecs/components/TextComponent.hpp
            ./ecs/components/TransformComponent.hpp
            ./ecs/components/VelocityComponent.hpp
            ./ecs/systems/ButtonSystem.hpp
            ./ecs/systems/LayoutSystem.hpp
            ./ecs/systems/MovementSystem.hpp
            ./ecs/systems/SpriteRenderSystem.hpp
            ./ecs/systems/TextRenderSystem.hpp
)

target_compile_features(${NAME} PRIVATE cxx_std_23)

if(MSVC)
    target_compile_options(${NAME} PRIVATE
        /external:W0
        /external:anglebrackets
        /external:templates-
    )
endif()

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
    PRIVATE
        ${Stb_INCLUDE_DIR}
)
