#ifndef SFA_SRC_ENGINE_UTILITY_UI_SIZE_HPP
#define SFA_SRC_ENGINE_UTILITY_UI_SIZE_HPP

#include <cstdint>

namespace sfa
{

/// \brief Abstraction over different ways to describe elements placements.
///
/// \author Felix Hommel
/// \date 1/23/2026
struct Size
{
    /// \brief What kind of measurement unit the size describes.
    ///
    /// Pixels: Are an absolute value (i.e., 200px)
    /// Percent: Are a relative value (i.e., 50%)
    /// Content: Fit to content (i.e., useful for text)
    ///
    /// \author Felix Hommel
    /// \date 1/23/2026
    enum class Type : std::uint8_t
    {
        Pixels,
        Percent,
        Content
    };

    Type type;
    float value;

    /// \brief Create a new size in pixels.
    ///
    /// \param px How many pixels the size measures
    ///
    /// \returns \ref Size with the pixel size
    static Size pixels(float px) { return { .type = Type::Pixels, .value = px }; }
    /// \brief Create a new size in percent.
    ///
    /// \param pct Percentage of the size
    ///
    /// \returns \ref Size with the percentage size
    static Size percent(float pct) { return { .type = Type::Percent, .value = pct }; }
    /// \brief Create a new fit to content size.
    ///
    /// \returns \ref Size where the element decides the actual size
    static Size content() { return { .type = Type::Content, .value = 0 }; }

    /// \brief Convert the \ref Size to actual pixels.
    ///
    /// \param parentSize Size of the parent element in pixels
    ///
    /// \returns Actual size in pixels
    [[nodiscard]] float resolveToPixels(float parentSize) const
    {
        switch (type)
        {
            case Type::Pixels: return value;
            case Type::Percent: return parentSize * value;
            case Type::Content: return 0;
        }
    }
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_UI_SIZE_HPP

