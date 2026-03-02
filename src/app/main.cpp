#include "core/Shader.hpp"
#include "core/SpriteRenderer.hpp"
#include "core/TextRenderer.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/UIButtonComponent.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UILayoutComponent.hpp"
#include "ecs/components/UILayoutElementComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"
#include "ecs/systems/ButtonSystem.hpp"
#include "ecs/systems/LayoutSystem.hpp"
#include "ecs/systems/UIRenderSystem.hpp"
#include "ecs/systems/UITransformSystem.hpp"
#include "utility/GLFWWindow.hpp"
#include "utility/userInput/InputController.hpp"
#include "utility/userInput/InputEvent.hpp"

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{

constexpr auto WINDOW_WIDTH{ 900 };
constexpr auto WINDOW_HEIGHT{ 700 };

std::string loadTextFile(const std::filesystem::path& path)
{
    std::ifstream file{ path };
    if(!file.is_open())
        throw std::runtime_error("Failed to open file: " + path.string());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


} // namespace

// NOLINTBEGIN(bugprone-*, readability-*):
int main()
{
    using namespace sfa;

    GLFWWindow window{ "SFA UI Sample", WINDOW_WIDTH, WINDOW_HEIGHT };

    auto input{ std::make_shared<InputController>() };
    window.attachInputController(input);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto spriteVertSrc{ loadTextFile(SFA_ROOT "resources/shaders/button.vert") };
    const auto spriteFragSrc{ loadTextFile(SFA_ROOT "resources/shaders/button.frag") };
    const auto textVertSrc{ loadTextFile(SFA_ROOT "resources/shaders/text.vert") };
    const auto textFragSrc{ loadTextFile(SFA_ROOT "resources/shaders/text.frag") };

    auto spriteShader{ std::make_shared<Shader>(spriteVertSrc.c_str(), spriteFragSrc.c_str()) };
    auto textShader{ std::make_shared<Shader>(textVertSrc.c_str(), textFragSrc.c_str()) };

    auto spriteRenderer{ std::make_shared<SpriteRenderer>(spriteShader) };
    auto textRenderer{ std::make_shared<TextRenderer>(textShader) };
    textRenderer->load(SFA_ROOT "resources/fonts/prstart.ttf", 18);
    UIRenderSystem uiRenderer{ spriteRenderer, textRenderer };

    ComponentRegistry registry;

    constexpr EntityID rootEntity{ 1 };
    constexpr EntityID playButtonEntity{ 2 };
    constexpr EntityID quitButtonEntity{ 3 };


    registry.addComponent<UITransformComponent>(
        rootEntity,
        { .localPosition = { 0.f, 0.f }, .worldPosition = { 0.f, 0.f }, .size = { WINDOW_WIDTH, WINDOW_HEIGHT } }
    );
    registry.addComponent<UIHierarchyComponent>(
        rootEntity,
        { .parent = NULL_ENTITY, .children = { playButtonEntity, quitButtonEntity } }
    );
    registry.addComponent<UILayoutComponent>(
        rootEntity,
        { .type = UILayoutComponent::Type::Vertical, .spacing = 30.f, .padding = { 250.f, 220.f } }
    );

    registry.addComponent<UITransformComponent>(
        playButtonEntity,
        { .localPosition = { 0.f, 0.f }, .worldPosition = { 0.f, 0.f }, .size = { 240.f, 80.f } }
    );
    registry.addComponent<UIHierarchyComponent>(playButtonEntity, { .parent = rootEntity, .children = {} });
    registry.addComponent<UILayoutElementComponent>(
        playButtonEntity,
        { .preferredSize = { 240.f, 80.f }, .flexGrow = 0.f }
    );
    registry.addComponent<SpriteComponent>(
        playButtonEntity,
        { .texture = nullptr, .size = { 240.f, 80.f }, .color = { 0.f, 1.f, 0.f }, .renderLayer = 0 }
    );
    registry.addComponent<TextComponent>(
        playButtonEntity,
        { .content = "PLAY", .offset = { 0.f, 0.f }, .scale = 1.f, .color = { 1.f, 1.f, 1.f }, .centerInTransform = true, .renderLayer = 1 }
    );

    UIButtonComponent playButton;
    playButton.standardColor = { 0.f, 0.f, 1.f };
    playButton.pressCooldownMax = 0.2f;
    playButton.onClick = [] { spdlog::info("PLAY pressed"); };
    registry.addComponent<UIButtonComponent>(playButtonEntity, playButton);

    registry.addComponent<UITransformComponent>(
        quitButtonEntity,
        { .localPosition = { 0.f, 0.f }, .worldPosition = { 0.f, 0.f }, .size = { 240.f, 80.f } }
    );
    registry.addComponent<UIHierarchyComponent>(quitButtonEntity, { .parent = rootEntity, .children = {} });
    registry.addComponent<UILayoutElementComponent>(
        quitButtonEntity,
        { .preferredSize = { 240.f, 80.f }, .flexGrow = 0.f }
    );
    registry.addComponent<SpriteComponent>(
        quitButtonEntity,
        { .texture = nullptr, .size = { 240.f, 80.f }, .color = { 1.f, 0.f, 0.f }, .renderLayer = 0 }
    );
    registry.addComponent<TextComponent>(
        quitButtonEntity,
        { .content = "QUIT", .offset = { 0.f, 0.f }, .scale = 1.f, .color = { 1.f, 1.f, 1.f }, .centerInTransform = true, .renderLayer = 1 }
    );

    UIButtonComponent quitButton;
    quitButton.standardColor = { 0.f, 0.f, 1.f };
    quitButton.pressCooldownMax = 0.2f;
    quitButton.onClick = [] { spdlog::info("QUIT pressed"); };
    registry.addComponent<UIButtonComponent>(quitButtonEntity, quitButton);

    float lastTime{ static_cast<float>(glfwGetTime()) };

    while(!window.shouldClose())
    {
        const float now{ static_cast<float>(glfwGetTime()) };
        const float dt{ now - lastTime };
        lastTime = now;

        input->processEventQueue();

        if(input->isKeyPressed(Key::Esc) == InputAction::Press)
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);

        const bool mousePressed{ input->isMousePressed(MouseButton::Left) == InputAction::Press };

        glClearColor(0.08f, 0.08f, 0.12f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        LayoutSystem::update(registry);
        UITransformSystem::update(registry);
        ButtonSystem::update(registry, dt, input->mousePosition(), mousePressed);
        uiRenderer.render(registry);

        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }

    return 0;
}

// NOLINTEND(bugprone-*, readability-*):

