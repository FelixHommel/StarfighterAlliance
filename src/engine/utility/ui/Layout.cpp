#include "Layout.hpp"

#include "glm/glm.hpp"
#include "utility/TextRenderer.hpp"

#include <iostream>
#include <memory>
#include <utility>

namespace sfa
{

Layout::Layout(const glm::vec2& position, const glm::vec2& size)
    : m_position(position)
    , m_size(size)
{}

void Layout::setPosition(const glm::vec2& pos)
{
    std::cout << "position set to: " << pos.x << ", " << pos.y << "\n";
    m_position = pos;
    recalculateLayout();
}

void Layout::setSize(const glm::vec2& size)
{
    m_size = size;
    recalculateLayout();
}

void Layout::addChildren(std::shared_ptr<LayoutElement> child)
{
    m_children.emplace_back(std::move(child));
    recalculateLayout();
}

void Layout::update(float dt)
{
    for(auto& c : m_children)
        c->update(dt);
}

void Layout::render(TextRenderer* textRenderer)
{
    for(auto& c : m_children)
        c->render(textRenderer);
}

} // namespace sfa

