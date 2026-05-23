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
#include "screens/MenuScreen.hpp"
#include "screens/ScreenStack.hpp"
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

    GLFWWindow window{ "Starfighter Alliance", WINDOW_WIDTH, WINDOW_HEIGHT };

    InputController input{};
    window.attachInputController(&input);

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
    ScreenStack screens{ std::make_unique<MenuScreen>(window, [] {}, [] {}) };

    float lastTime{ static_cast<float>(glfwGetTime()) };

    while(!window.shouldClose())
    {
        const float now{ static_cast<float>(glfwGetTime()) };
        const float dt{ now - lastTime };
        lastTime = now;

        input.processEventQueue();
        screens.processCommands();

        if(input.isKeyPressed(Key::Esc) == InputAction::Press)
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);

        glClearColor(0.08f, 0.08f, 0.12f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        screens.update(dt, input);
        screens.render({ .uiRenderer = uiRenderer, .window = window });

        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }

    return 0;
}

// NOLINTEND(bugprone-*, readability-*):

