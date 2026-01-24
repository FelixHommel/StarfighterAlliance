#ifndef SFA_SRC_ENGINE_CORE_BUTTON_HPP
#define SFA_SRC_ENGINE_CORE_BUTTON_HPP

#include "Shader.hpp"
#include "TextRenderer.hpp"
#include "core/ui/LayoutElement.hpp"

#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>

namespace sfa
{

/// \brief Buttons are a UI element to get user input.
///
/// This class abstracts Buttons to a point where only the position and size needs to be defined and then the Button
/// can be rendered via the member draw() function.
///
/// \author Felix Hommel
/// \date 11/16/2024
class Button final : public LayoutElement
{
public:
    Button(
        const glm::vec2& pos,
        const glm::vec2& size,
        const glm::vec3& defaultColor,
        const glm::vec3& hoveredColor,
        const glm::vec3& pressedColor,
        const std::string& label,
        std::shared_ptr<Shader> shader,
        float pressCooldownMax = 0.25f
    );
    ~Button() override;

    Button(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = delete;

    [[nodiscard]] glm::vec2 position() const override { return m_position; }
    [[nodiscard]] glm::vec2 size() const override { return m_size; }

    void setPosition(const glm::vec2& pos) override { m_position = pos; }
    void setSize(const glm::vec2& size) override { m_size = size; }

    void update(float mouseX, float mouseY, bool mousePressed, float dt = 0.f);
    void draw(TextRenderer* renderer = nullptr);

    void update(float dt) override;
    void render(TextRenderer* textRenderer) override;

    void setOnClick(std::function<void()> callback) { m_onClick = callback; }

private:
    static constexpr std::size_t BUTTON_VERTICES{ 6 };
    static constexpr std::size_t BUTTON_VERTEX_ATTRIBUTES{ 4 };
    static constexpr std::array<float, BUTTON_VERTICES * BUTTON_VERTEX_ATTRIBUTES> VERTICES{
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    std::shared_ptr<Shader> m_shader;
    unsigned int m_vao{ 0 };

    glm::vec2 m_position;
    glm::vec2 m_size;
    glm::vec3 m_defaultColor;
    glm::vec3 m_hoveredColor;
    glm::vec3 m_pressedColor;
    glm::vec3 m_renderColor;
    std::string m_label;

    std::function<void()> m_onClick;
    float m_pressCooldownMax;
    float m_pressCooldown{ 0 };
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_BUTTON_HPP
