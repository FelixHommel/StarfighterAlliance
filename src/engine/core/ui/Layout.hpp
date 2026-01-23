#ifndef SFA_SRC_ENGINE_UTILITY_UI_LAYOUT_HPP
#define SFA_SRC_ENGINE_UTILITY_UI_LAYOUT_HPP

#include "Anchor.hpp"
#include "LayoutElement.hpp"
#include "core/TextRenderer.hpp"

#include <memory>
#include <vector>

namespace sfa
{

class Layout : public LayoutElement
{
public:
    Layout(const glm::vec2& position, const glm::vec2& size);
    ~Layout() override = default;

    Layout(const Layout&) = delete;
    Layout(Layout&&) = delete;
    Layout& operator=(const Layout&) = delete;
    Layout& operator=(Layout&&) = delete;

    [[nodiscard]] glm::vec2 position() const override { return m_position; }
    [[nodiscard]] glm::vec2 size() const override { return m_size; }

    void setPosition(const glm::vec2& pos) override;
    void setSize(const glm::vec2& size) override;

    Layout& anchor(Anchor a) { m_anchor = a; return *this; }
    Layout& padding(float p) { m_padding = glm::vec2(p); return *this; }
    Layout& margin(float m) { m_margin = glm::vec2(m); return *this; }

    void addChildren(std::shared_ptr<LayoutElement> child);
    virtual void recalculateLayout() = 0;

    void update(float dt) override;
    void render(TextRenderer* textRenderer) override;

protected:
    std::vector<std::shared_ptr<LayoutElement>> m_children;
    glm::vec2 m_position;
    glm::vec2 m_size;
    Anchor m_anchor{ Anchor::TOP_LEFT };
    glm::vec2 m_padding{ glm::vec2(0.f) };
    glm::vec2 m_margin{ glm::vec2(0.f) };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_UI_LAYOUT_HPP

