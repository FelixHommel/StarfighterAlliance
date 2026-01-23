#ifndef SFA_SRC_ENGINE_UTILITY_UI_VERTICAL_LAYOUT_HPP
#define SFA_SRC_ENGINE_UTILITY_UI_VERTICAL_LAYOUT_HPP

#include "Layout.hpp"
#include <glm/ext/vector_float2.hpp>

namespace sfa
{

class VerticalLayout : public Layout
{
public:
    VerticalLayout(const glm::vec2& position, const glm::vec2& size, float spacing = DEFAULT_SPACING);
    ~VerticalLayout() override = default;

    VerticalLayout& spacing(float s);

    VerticalLayout(const VerticalLayout&) = delete;
    VerticalLayout(VerticalLayout&&) = delete;
    VerticalLayout& operator=(const VerticalLayout&) = delete;
    VerticalLayout& operator=(VerticalLayout&&) = delete;

    void recalculateLayout() override;

private:
    static constexpr auto DEFAULT_SPACING{ 10.f };

    float m_spacing;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_UI_VERTICAL_LAYOUT_HPP

