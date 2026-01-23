#include "VerticalLayout.hpp"

#include "utility/ui/Anchor.hpp"
#include "utility/ui/Layout.hpp"

#include "glm/glm.hpp"
#include <cstddef>

namespace sfa
{

VerticalLayout::VerticalLayout(const glm::vec2& position, const glm::vec2& size, float spacing)
    : Layout(position, size)
    , m_spacing(spacing)
{}

VerticalLayout& VerticalLayout::spacing(float s)
{
    m_spacing = s;
    recalculateLayout();

    return *this;
}

void VerticalLayout::recalculateLayout()
{
    if(m_children.empty())
        return;

    glm::vec2 availableSpace{ m_size - 2.f * m_padding };

    float totalHeight{ 0.f };
    for(std::size_t i{ 0 }; i < m_children.size(); ++i)
    {
        totalHeight += m_children[i]->size().y;

        if(i > 0)
            totalHeight += m_spacing;
    }

    glm::vec2 currentPos{ m_position + m_padding };

    if(m_anchor == Anchor::CENTER || m_anchor == Anchor::CENTER_LEFT || m_anchor == Anchor::CENTER_RIGHT)
        currentPos.y += (availableSpace.y - totalHeight) * 0.5f;

    for(auto& c : m_children)
    {
        glm::vec2 childSize{ c->size() };
        glm::vec2 childPos{ currentPos };

        switch(m_anchor)
        {
            case Anchor::TOP_CENTER:
            case Anchor::CENTER:
            case Anchor::BOTTOM_CENTER:
                childPos.x += (availableSpace.x - childPos.x) * 0.5f;
                break;
            case Anchor::TOP_RIGHT:
            case Anchor::CENTER_RIGHT:
            case Anchor::BOTTOM_RIGHT:
                childPos.x += availableSpace.x - childPos.x;
                break;
            default:
                break;
        }

        c->setPosition(childPos);
        currentPos.y += childSize.y + m_spacing;
    }
}

} // namespace sfa

