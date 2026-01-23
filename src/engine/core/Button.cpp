#include "Button.hpp"

#include "Shader.hpp"
#include "core/TextRenderer.hpp"

#include "glad/gl.h"
#include "glm/ext/matrix_transform.hpp"

#include <array>
#include <string>

namespace sfa
{

Button::Button(
    const glm::vec2& pos,
    const glm::vec2& size,
    const glm::vec3& defaultColor,
    const glm::vec3& hoveredColor,
    const glm::vec3& pressedColor,
    const std::string& label,
    const Shader& shader,
    float pressCooldownMax
)
    : m_shader(shader)
    , m_vao(0)
    , m_position(pos)
    , m_size(size)
    , m_defaultColor(defaultColor)
    , m_hoveredColor(hoveredColor)
    , m_pressedColor(pressedColor)
    , m_renderColor(defaultColor)
    , m_label(label)
    , m_pressCooldownMax(pressCooldownMax)
    , m_pressCooldown(0)
{
    glGenVertexArrays(1, &m_vao);
    unsigned int VBO{ 0 };
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    glVertexAttribPointer(
        0,
        BUTTON_VERTEX_ATTRIBUTES,
        GL_FLOAT,
        GL_FALSE,
        BUTTON_VERTEX_ATTRIBUTES * sizeof(float),
        nullptr
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

Button::~Button()
{
    glDeleteVertexArrays(1, &m_vao);
}

void Button::update(float mouseX, float mouseY, bool mousePressed, float dt)
{
    if(mouseX >= m_position.x && mouseX <= m_position.x + m_size.x && mouseY >= m_position.y
       && mouseY <= m_position.y + m_size.y)
    {
        if(m_onClick && mousePressed && m_pressCooldown <= 0)
        {
            m_renderColor = m_pressedColor;
            m_onClick();
            m_pressCooldown = m_pressCooldownMax;
        }
        else
            m_renderColor = m_hoveredColor;
    }
    else
        m_renderColor = m_defaultColor;

    if(m_pressCooldown > 0)
        m_pressCooldown -= dt;
}

void Button::draw(TextRenderer* renderer)
{
    m_shader.use();
    auto model{ glm::mat4(1.0f) };

    model = glm::translate(model, glm::vec3(m_position, 0.0f));
    model = glm::scale(model, glm::vec3(m_size, 1.0f));

    m_shader.setMatrix4("model", model);
    m_shader.setVector3f("color", m_renderColor);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, BUTTON_VERTICES);

    glBindVertexArray(0);

    static constexpr float BUTTON_LABEL_X_OFFSET{ 10.f };
    static constexpr float BUTTON_LABEL_Y_OFFSET{ 5.f };
    if(renderer != nullptr)
        renderer->draw(m_label, m_position.x + BUTTON_LABEL_X_OFFSET, (m_position.y + m_size.y / 2.f) - BUTTON_LABEL_Y_OFFSET);
}

void Button::update(float dt)
{
}

void Button::render(TextRenderer* textRenderer)
{
    draw(textRenderer);
}

} // namespace sfa

