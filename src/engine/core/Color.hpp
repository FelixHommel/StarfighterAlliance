#ifndef CORE_COLOR_HPP
#define CORE_COLOR_HPP

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

/*
 * @brief Enmumeration of all Colors that can be recieved from the server
 */
enum class ColorType : uint8_t
{
    red,
    blue,
    yellow,
    orange,
    green,
    violet,
    white,
    black,
    transparent
};

namespace Color
{

static std::string toString(ColorType color)
{
    switch(color)
    {
    case ColorType::red:
        return "red";
    case ColorType::blue:
        return "blue";
    case ColorType::yellow:
        return "yellow";
    case ColorType::orange:
        return "orange";
    case ColorType::green:
        return "green";
    case ColorType::violet:
        return "violet";
    case ColorType::white:
        return "white";
    case ColorType::black:
        return "black";
    case ColorType::transparent:
        return "transparent";
    }
}

static ColorType toEnum(std::string_view color)
{
    if(color == "RED")
        return ColorType::red;
    else if(color == "BLUE")
        return ColorType::blue;
    else if(color == "YELLOW")
        return ColorType::yellow;
    else if(color == "ORANGE")
        return ColorType::orange;
    else if(color == "GREEN")
        return ColorType::green;
    else if(color == "VIOLET")
        return ColorType::violet;
    else if(color == "WHITE")
        return ColorType::white;
    else if(color == "BLACK")
        return ColorType::black;
    else if(color == "TRANSPARENT")
        return ColorType::transparent;

    std::stringstream errorMsg;
    errorMsg << "Invalid argument supplied to Color::toEnum. " << color << " is not a valid argument";
    throw std::runtime_error(errorMsg.str());
}

}; // namespace Color

#endif //! CORE_COLOR_HPP
