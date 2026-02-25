#include "utility/GLFWWindow.hpp"

namespace
{

constexpr auto TEMP_WINDOW_WIDTH{ 600 };
constexpr auto TEMP_WINDOW_HEIGHT{ 500 };

} // namespace

int main()
{
    using namespace sfa;

    GLFWWindow window{ "HelloWorld", TEMP_WINDOW_WIDTH, TEMP_WINDOW_HEIGHT };

    while(!window.shouldClose())
    {
    }

    return 0;
}
