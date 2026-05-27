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
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{

constexpr auto WINDOW_WIDTH{ 1200 };
constexpr auto WINDOW_HEIGHT{ 800 };

std::string loadTextFile(const std::filesystem::path& path)
{
    std::ifstream file{ path };
    if(!file.is_open())
        throw std::runtime_error("Failed to open file: " + path.string());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void addLayoutElement(
    sfa::ComponentRegistry& registry, const sfa::EntityID entity, const glm::vec2& preferredSize, const float flexGrow
)
{
    registry.addComponent<sfa::UILayoutElementComponent>(
        entity,
        {
            .preferredSize = preferredSize,
            .flexGrow = flexGrow,
        }
    );
}

void addButton(
    sfa::ComponentRegistry& registry,
    const sfa::EntityID entity,
    const sfa::EntityID parent,
    const glm::vec2& size,
    std::string text,
    const glm::vec3& color,
    const std::function<void()>& onClick
)
{
    registry.addComponent<sfa::UITransformComponent>(
        entity,
        {
            .localPosition = { 0.f, 0.f },
              .worldPosition = { 0.f, 0.f },
              .size = size
    }
    );
    registry.addComponent<sfa::UIHierarchyComponent>(entity, { .parent = parent, .children = {} });
    addLayoutElement(registry, entity, size, 0.f);
    registry.addComponent<sfa::SpriteComponent>(
        entity,
        {
            .texture = nullptr,
            .size = size,
            .color = color,
            .renderLayer = 0,
        }
    );
    registry.addComponent<sfa::TextComponent>(
        entity,
        {
            .content = std::move(text),
            .offset = { 0.f, 0.f },
            .scale = 0.8f,
            .color = { 1.f, 1.f, 1.f },
            .centerInTransform = true,
            .renderLayer = 1,
    }
    );

    sfa::UIButtonComponent button;
    button.standardColor = color;
    button.pressCooldownMax = 0.2f;
    button.onClick = onClick;
    registry.addComponent<sfa::UIButtonComponent>(entity, button);
}

} // namespace

// NOLINTBEGIN(bugprone-*, readability-*):
int main()
{
    using namespace sfa;

    GLFWWindow window{ "SFA Complex UI Sample", WINDOW_WIDTH, WINDOW_HEIGHT };

    auto input{ std::make_shared<InputController>() };
    window.attachInputController(input.get());

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

    constexpr EntityID rootEntity{ 1 };
    constexpr EntityID centerArea{ 2 };
    constexpr EntityID centerRow{ 3 };
    constexpr EntityID bottomBar{ 4 };
    constexpr EntityID centerSpriteEntity{ 5 };
    constexpr EntityID leftButtonEntity{ 6 };
    constexpr EntityID rightButtonEntity{ 7 };
    constexpr EntityID leftBottomButtonEntity{ 8 };
    constexpr EntityID rightBottomButtonEntity{ 9 };

    ComponentRegistry registry;

    // ROOT
    registry.addComponent<UITransformComponent>(
        rootEntity,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT),
        }
    );
    registry.addComponent<UIHierarchyComponent>(
        rootEntity,
        {
            .parent = NULL_ENTITY,
            .children = { centerArea, bottomBar },
    }
    );
    registry.addComponent<UILayoutComponent>(
        rootEntity,
        {
            .type = UILayoutComponent::Type::Vertical,
            .spacing = 20.f,
            .padding = glm::vec2(20.f),
        }
    );

    // CENTER AREA
    registry.addComponent<UITransformComponent>(
        centerArea,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = glm::vec2(WINDOW_WIDTH - 40.f, WINDOW_HEIGHT - 160.f),
        }
    );
    registry.addComponent<UIHierarchyComponent>(
        centerArea,
        {
            .parent = rootEntity,
            .children = { centerRow },
        }
    );
    registry.addComponent<UILayoutComponent>(
        centerArea,
        {
            .type = UILayoutComponent::Type::Vertical,
            .spacing = 0.f,
            .padding = glm::vec2(0.f),
        }
    );

    // CENTER ROW
    registry.addComponent<UITransformComponent>(
        centerRow,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = glm::vec2(WINDOW_WIDTH - 40.f, 400.f),
        }
    );
    registry.addComponent<UIHierarchyComponent>(
        centerRow,
        {
            .parent = centerArea,
            .children = { leftButtonEntity, centerSpriteEntity, rightButtonEntity },
    }
    );
    registry.addComponent<UILayoutComponent>(
        centerRow,
        {
            .type = UILayoutComponent::Type::Horizontal,
            .spacing = 40.f,
            .padding = glm::vec2(40.f),
        }
    );

    // PREVIEW SPRITE
    constexpr glm::vec2 centerSpriteSize{ 300.f, 300.f };
    registry.addComponent<UITransformComponent>(
        centerSpriteEntity,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = centerSpriteSize,
        }
    );
    registry.addComponent<UIHierarchyComponent>(
        centerSpriteEntity,
        {
            .parent = centerRow,
            .children = {},
        }
    );
    addLayoutElement(registry, centerSpriteEntity, centerSpriteSize, 0.f);
    registry.addComponent<SpriteComponent>(
        centerSpriteEntity,
        {
            .texture = nullptr,
            .size = centerSpriteSize,
            .color = glm::vec3(0.95f, 0.75f, 0.2f),
            .renderLayer = 0,
        }
    );
    registry.addComponent<TextComponent>(
        centerSpriteEntity,
        {
            .content = "SHIP",
            .offset = glm::vec2(0.f),
            .scale = 0.9f,
            .color = glm::vec3(0.1f, 0.1f, 0.1f),
            .centerInTransform = true,
            .renderLayer = 1,
        }
    );

    // SIDE BUTTONS
    addButton(registry, leftButtonEntity, centerRow, { 120.f, 70.f }, "<", { 0.2f, 0.55f, 0.95f }, [] {
        spdlog::info("LEFT pressed");
    });
    addButton(registry, rightButtonEntity, centerRow, { 120.f, 70.f }, ">", { 0.3f, 0.75f, 0.35f }, [] {
        spdlog::info("RIGHT pressed");
    });

    // BOTTOM BAR
    registry.addComponent<UITransformComponent>(
        bottomBar,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = glm::vec2(WINDOW_WIDTH - 40.f, 100.f),
        }
    );

    registry.addComponent<UIHierarchyComponent>(
        bottomBar,
        {
            .parent = rootEntity,
            .children = { leftBottomButtonEntity, rightBottomButtonEntity },
    }
    );

    registry.addComponent<UILayoutComponent>(
        bottomBar,
        {
            .type = UILayoutComponent::Type::Horizontal,
            .spacing = WINDOW_WIDTH - 520.f,
            .padding = glm::vec2(20.f),
        }
    );

    // BOTTOM BUTTONS
    addButton(registry, leftBottomButtonEntity, bottomBar, { 220.f, 70.f }, "BACK", { 0.85f, 0.35f, 0.35f }, [] {
        spdlog::info("BACK pressed");
    });
    addButton(registry, rightBottomButtonEntity, bottomBar, { 220.f, 70.f }, "SELECT", { 0.6f, 0.4f, 0.9f }, [] {
        spdlog::info("SELECT pressed");
    });

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
        uiRenderer.render(registry, window.viewport());

        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }

    return 0;
}

// NOLINTEND(bugprone-*, readability-*):

